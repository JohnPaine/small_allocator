#include "voidptrpool.h"
#include "voidptrpooliterator.h"
#include <QDebug>

VoidPtrBlock::VoidPtrBlock(VoidPtrBlock* nextInList) : next(nextInList)
{
    for (auto i = 0; i < BLOCK_SIZE - 1; ++i)
    {
        poolSlots[i].m_address = &poolSlots[i + 1];
    }
    poolSlots[BLOCK_SIZE - 1].m_address = nullptr;
}

VoidPtrBlock::~VoidPtrBlock()
{
    if (next)
    {
        delete next;
    }
}

VoidPtrPool::VoidPtrPool()
{
}

VoidPtrPool::~VoidPtrPool()
{
    if (block_list)
    {
        delete block_list;
    }
}

VoidPtr* VoidPtrPool::Allocate()
{
    if (!free_list)
    {
        if (block_list)
        {
            qDebug() << "ERROR!!! VoidPtrBlock is initialized again!";
        }
        /// TODO: CHECK!!!!

        block_list = new VoidPtrBlock(block_list);
        block_list->poolSlots[BLOCK_SIZE - 1].m_address = free_list;
        free_list = &block_list->poolSlots[0];
    }
    VoidPtr* space = (VoidPtr*)free_list;
    free_list = (VoidPtr*)space->m_address;
    return space;
}

void VoidPtrPool::Deallocate(VoidPtr* vp)
{
    vp->m_address = free_list;
    free_list = (VoidPtr*)vp->m_address;
    vp->m_size = 0;
}

VoidPtrIterator* VoidPtrPool::iterator()
{
    return new VoidPtrPoolIterator(block_list);
}

