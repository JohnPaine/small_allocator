#include "smallallocator.h"

#include <bitset>
#include <QDebug>
#include <iostream>

#define SMALL_ALLOC_SIZE 0x100000

#define DATA_OFFSET 5 // 1 byte for isOccupied flag, 4 bytes for Size
#define SIZE_OFFSET 1 // 1 byte for isOccupied flag

#define MEM_BLOCK_SIZE sizeof(char)
#define SIZE_NUM_BYTES sizeof(unsigned int)

/// DATA ALLOC SCHEME:
/// <0 byte -- isOccupied>, <SIZE_OFFSET : (DATA_OFFSET - SIZE_OFFSET) bytes -- Size (LSB)>, <DATA_OFFSET : (Size + DATA_OFFSET) bytes -- DATA>

// Other solutions:
// https://stepik.org/lesson/12574/step/7?thread=solutions&unit=3002































void SmallAllocator::printBytes(int startFrom, int howMany) const
{
    howMany = howMany < 0 ? SMALL_ALLOC_SIZE - startFrom : howMany;
    const auto toStop = startFrom + howMany;

    if (startFrom < 0 || startFrom >= SMALL_ALLOC_SIZE || toStop <= startFrom || toStop >= SMALL_ALLOC_SIZE)
    {
        return;
    }

    std::cout << QString("Memory [%1-%2]:").arg(startFrom).arg(toStop).toStdString();
    for (auto i = startFrom; i < toStop; ++i)
    {
        std::cout << int(Memory[i]) << ",";
    }
    std::cout << std::endl;
}

void SmallAllocator::printBytes(void* spacePtr) const
{
    const auto isOccupiedPos = getBlockStartPos(spacePtr);
    const auto fullSize = getFullBlockSize(spacePtr);
    if (fullSize > DATA_OFFSET)
    {
        printBytes(isOccupiedPos, fullSize);
    }
}

void SmallAllocator::printAllBlocks() const
{
    std::cout << "Printing all blocks..." << std::endl;
    for (auto isOccupiedPos = 0; isOccupiedPos < SMALL_ALLOC_SIZE - DATA_OFFSET;)
    {
        const auto spacePtr = getSpacePtr(isOccupiedPos);
        printBytes(spacePtr);
        const auto fullSize = getFullBlockSize(spacePtr);
        if (fullSize <= DATA_OFFSET)
        {
            break;
        }
        isOccupiedPos = getNextBlockStartPos(isOccupiedPos);
    }
}














