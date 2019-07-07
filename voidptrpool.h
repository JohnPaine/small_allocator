#ifndef VOIDPTRPOOL_H
#define VOIDPTRPOOL_H

#include <cstddef>
#include "voidptr.h"

#define BLOCK_SIZE 128

class VoidPtrIterator;

struct VoidPtrBlock
{
    VoidPtrBlock* next {nullptr};
    VoidPtr poolSlots[BLOCK_SIZE];

    VoidPtrBlock(VoidPtrBlock* nextInList);
    ~VoidPtrBlock();
};

class VoidPtrPool
{
    friend class VoidPtrIterator;

    VoidPtr* free_list {nullptr};
    VoidPtrBlock* block_list {nullptr};
public:
    VoidPtrPool();
    ~VoidPtrPool();

    VoidPtr* Allocate();
    void Deallocate(VoidPtr* vp);

    VoidPtrIterator* iterator();
};

#endif // VOIDPTRPOOL_H
