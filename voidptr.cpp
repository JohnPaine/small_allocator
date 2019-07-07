#include "voidptr.h"
#include "voidptrpool.h"
#include "space.h"

VoidPtrPool* VoidPtr::s_pool = nullptr;
Space* VoidPtr::s_space = new Space();

VoidPtr::VoidPtr()
{

}

VoidPtr::VoidPtr(void* adr, size_t s) : m_address(adr), m_size(s)
{

}

VoidPtr::~VoidPtr()
{

}

void* VoidPtr::operator new (size_t)
{
    if (!s_pool)
    {
        s_pool = new VoidPtrPool;
    }
    return s_pool->Allocate();
}

void* VoidPtr::operator new (size_t size, Space* space)
{
    return space->Allocate(size);
}

void VoidPtr::operator delete (void* space)
{
    s_pool->Deallocate((VoidPtr*)space);
}

void VoidPtr::Grab()
{
    m_refCount++;
}

void VoidPtr::Release()
{
    if (m_refCount > 0)
    {
        m_refCount--;
    }
    if (m_refCount <= 0)
    {
        delete this;
    }
}
