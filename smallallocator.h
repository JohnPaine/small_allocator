#ifndef SMALLALLOC_H
#define SMALLALLOC_H

#include <cstddef>
#include <cassert>
#include <cstring>

#define SMALL_ALLOC_SIZE 0x100000

#define DATA_OFFSET 5 // 1 byte for isOccupied flag, 4 bytes for Size
#define SIZE_OFFSET 1 // 1 byte for isOccupied flag

#define MEM_BLOCK_SIZE sizeof(char)
#define SIZE_NUM_BYTES sizeof(unsigned int)

/// DATA ALLOC SCHEME:
/// <0 byte -- isOccupied>, <SIZE_OFFSET : (DATA_OFFSET - SIZE_OFFSET) bytes -- Size (LSB)>, <DATA_OFFSET : (Size + DATA_OFFSET) bytes -- DATA>

class SmallAllocator
{
    char* Memory{nullptr};

    int findPlaceForAlloc(const unsigned int spaceSize) {
        int availableBlockStart = -1;
        unsigned int availableSpaceSize = 0;
        for (auto isOccupiedPos = 0; isOccupiedPos < SMALL_ALLOC_SIZE - DATA_OFFSET;) {
            const auto occupied = isOccupied(isOccupiedPos);
            const auto currentSpaceSize = getSpaceSize(isOccupiedPos);
            if (occupied || (currentSpaceSize > 0 && currentSpaceSize < spaceSize)) {
                if (!occupied) {
                    auto bytesToAdd = currentSpaceSize;
                    if (availableBlockStart < 0) {
                        availableBlockStart = isOccupiedPos;
                    }
                    else {
                        // if it's not the first joining block in a series, we can use its utility bytes too
                        bytesToAdd += DATA_OFFSET + 1;
                    }
                    availableSpaceSize += bytesToAdd;
                    if (availableSpaceSize >= spaceSize) {
                        // we need to override the concatenated blocks so that the bytes sequence remained valid
                        saveState(availableBlockStart, availableSpaceSize, false);
                        return availableBlockStart;
                    }
                }
                else {
                    availableBlockStart = -1;
                    availableSpaceSize = 0;
                }
                isOccupiedPos = getNextBlockStartPos(isOccupiedPos);
                continue;
            }
            return isOccupiedPos;
        }
        return -1;
    }
    void saveState(const int isOccupiedPos, const unsigned int spaceSize, const bool occupied) {
        Memory[isOccupiedPos] = occupied;
        saveSize(isOccupiedPos + SIZE_OFFSET, spaceSize);
    }
    void saveSize(const int sizePos, const unsigned int spaceSize) {
        for (size_t i = 0; i < SIZE_NUM_BYTES; ++i) {
            Memory[sizePos + i] = getSizeBits(spaceSize, i);
        }
        const auto savedSize = getSpaceSize(sizePos - SIZE_OFFSET);
        assert(spaceSize == savedSize);
    }
    char getSizeBits(const unsigned int spaceSize, const int byteNum) const {
        return (spaceSize >> (8 * byteNum)) & 0xff;
    }
    int getBlockStartPos(const void* spacePtr) const {
        const ptrdiff_t pos = (ptrdiff_t(spacePtr) - ptrdiff_t(&Memory[0])) / MEM_BLOCK_SIZE - DATA_OFFSET;
        return int(pos);
    }
    int getNextBlockStartPos(const int isOccupiedPos) const {
        return isOccupiedPos + getFullBlockSize(isOccupiedPos) + 1;
    }
    unsigned int getSpaceSize(const int isOccupiedPos) const {
        unsigned int size = 0;
        const auto startFrom = isOccupiedPos + SIZE_OFFSET;
        for (size_t byteNum = 0; byteNum < SIZE_NUM_BYTES; ++byteNum) {
            const auto bytePos = startFrom + byteNum;
            size |= (Memory[bytePos] << (byteNum * 8));
        }
        return size;
    }
    bool checkSize(const int isOccupiedPos, const unsigned int spaceSize) const {
        return !(isOccupiedPos < 0 || isOccupiedPos + spaceSize + DATA_OFFSET >= SMALL_ALLOC_SIZE);
    }
    void* getSpacePtr(const int isOccupiedPos) const {
        return &Memory[isOccupiedPos + DATA_OFFSET];
    }
    bool trySplitBlocks(const int isOccupiedPos, const unsigned int expectedNewBlockFullSize) {
        // I guess we don't want the splitted block to be of size < 4 (too much total cost)
        if (expectedNewBlockFullSize < DATA_OFFSET + 4) {
            return false;
        }

        const auto currentSpaceSize = getSpaceSize(isOccupiedPos);
        // we can reduce only the space size, not the full size of the splitting block
        const auto reducedCurrentSize = currentSpaceSize - expectedNewBlockFullSize;
        const auto nextBlockSpaceSize = expectedNewBlockFullSize - DATA_OFFSET - 1;

        saveState(isOccupiedPos, reducedCurrentSize, true);
        assert(getSpaceSize(isOccupiedPos) == reducedCurrentSize);

        const auto nextBlockStart = getNextBlockStartPos(isOccupiedPos);

        saveState(nextBlockStart, nextBlockSpaceSize, false);
        assert(getSpaceSize(nextBlockStart) == nextBlockSpaceSize);

        return true;
    }
    void* clearData(void* spacePtr, const int spaceSize) const {
        return memset(spacePtr, 0, spaceSize);
    }
    unsigned int getFullBlockSize(const int isOccupiedPos) const {
        return getSpaceSize(isOccupiedPos) + DATA_OFFSET;
    }
    bool isLastEmptyBlock(const int isOccupiedPos) const {
        const auto nextIsOccupiedPos = getNextBlockStartPos(isOccupiedPos);
        return !isOccupied(nextIsOccupiedPos) && !getSpaceSize(nextIsOccupiedPos);
    }
    bool isOccupied(const int isOccupiedPos) const {
        return Memory[isOccupiedPos];
    }
    void clearBlock(const int isOccupiedPos, const unsigned int fullSize) {
        memset(&Memory[isOccupiedPos], 0, fullSize);
    }

public:
    void printBytes(int startFrom = 0, int howMany = 256) const;
    void printBytes(void* spacePtr) const;
    void printAllBlocks() const;

    unsigned int getSpaceSize(const void* spacePtr) const {
        const auto isOccupiedPos = getBlockStartPos(spacePtr);
        return getSpaceSize(isOccupiedPos);
    }
    unsigned int getFullBlockSize(const void* spacePtr) const {
        return getSpaceSize(spacePtr) + DATA_OFFSET;
    }
    bool isOccupied(const void* spacePtr) const {
        return isOccupied(getBlockStartPos(spacePtr));
    }

    SmallAllocator() {
        // () at the end are for the default-initialized array
        Memory = new char [SMALL_ALLOC_SIZE]();
    }
    void* Alloc(const unsigned int spaceSize) {
        const auto isOccupiedPos = findPlaceForAlloc(spaceSize);
        if (!checkSize(isOccupiedPos, spaceSize)) {
            // not enough memory
            return nullptr;
        }
        const auto currentSpaceSize = getSpaceSize(isOccupiedPos);
        if (currentSpaceSize > spaceSize && trySplitBlocks(isOccupiedPos, currentSpaceSize - spaceSize)) {
            assert(getSpaceSize(isOccupiedPos) == spaceSize);
        }
        else {
            saveState(isOccupiedPos, currentSpaceSize > 0 ? currentSpaceSize : spaceSize, true);
        }

        void* spacePtr = getSpacePtr(isOccupiedPos);
        return clearData(spacePtr, spaceSize);
    }
    void* ReAlloc(void* spacePtr, const unsigned int newSpaceSize) {
        if (!spacePtr && !newSpaceSize) {
            // the result is undefined?
            return nullptr;
        }
        if (!newSpaceSize) {
            Free(spacePtr);
            return nullptr;
        }
        if (!spacePtr) {
            return Alloc(newSpaceSize);
        }
        const auto isOccupiedPos = getBlockStartPos(spacePtr);
        if (!checkSize(isOccupiedPos, newSpaceSize)) {
            return nullptr;
        }
        const auto currentSpaceSize = getSpaceSize(isOccupiedPos);
        if (currentSpaceSize >= newSpaceSize) {
            if (trySplitBlocks(isOccupiedPos, currentSpaceSize - newSpaceSize)) {
                assert(getSpaceSize(isOccupiedPos) == newSpaceSize);
            }
            return spacePtr;
        }
        const auto newPtr = Alloc(newSpaceSize);
        if (newPtr) {
            memcpy(newPtr, spacePtr, currentSpaceSize * MEM_BLOCK_SIZE);
            Free(spacePtr);
        }
        return newPtr;
    }
    void Free(void* spacePtr) {
        const auto isOccupiedPos = getBlockStartPos(spacePtr);
        if (isOccupiedPos < 0 || isOccupiedPos >= SMALL_ALLOC_SIZE - DATA_OFFSET) {
            return;
        }
        const auto currentSpaceSize = getSpaceSize(isOccupiedPos);
        saveState(isOccupiedPos, currentSpaceSize, false);

        if (isLastEmptyBlock(isOccupiedPos)) {
            clearBlock(isOccupiedPos, getFullBlockSize(isOccupiedPos));
        }
        else {
            clearData(spacePtr, currentSpaceSize);
        }
    }
};

#endif // SMALLALLOC_H
