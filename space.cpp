#include "space.h"
#include <new>
#include <voidptr.h>
#include <voidptrpool.h>
#include <voidptrpooliterator.h>
#include <memory>

void Space::Swap()
{
    if (m_active == &A)
    {
        m_active = &B;
        m_inactive = &A;
    }
    else
    {
        m_active = &A;
        m_inactive = &B;
    }
    m_active->Reinitialize();
    // go through all VoidPtrs to find former active objects
    std::unique_ptr<VoidPtrIterator> it(VoidPtr::s_pool->iterator());
    while (it->More())
    {
        auto vp = it->Next();
        if (vp->m_address >= m_inactive && vp->m_address < m_inactive + sizeof(*m_inactive))
        {
            auto newSpace = m_active->Allocate(vp->m_size);
            if (!newSpace)
            {
                throw std::bad_alloc();
            }
            memcpy(newSpace, vp->m_address, vp->m_size);
            vp->m_address = newSpace;
        }
    }
}

Space::Space() : m_active(&A), m_inactive(&B)
{
}

void* Space::Allocate(size_t size)
{
    void* space = m_active->Allocate(size);
    if (space)
    {
        return space;
    }
    Swap();
    space = m_active->Allocate(size);
    if (!space)
    {
        throw std::bad_alloc();
    }
    return space;
}

void* HalfSpace::Allocate(size_t size)
{
    // round up to a word edge
    //    size = ROUNDUP(size);
    if (m_nextByte + size >= HALF_SPACE_SIZE)
    {
        // not enough memory
        return nullptr;
    }
    void* space = &m_bytes[m_nextByte];
    m_nextByte += size;
    return space;
}

void HalfSpace::Reinitialize()
{
    m_nextByte = 0;
}
