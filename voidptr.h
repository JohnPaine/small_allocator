#ifndef VOIDPTR_H
#define VOIDPTR_H

#include "space.h"

class VoidPtrPool;

class VoidPtr
{
    friend class VoidPtrPool;
    friend class VoidPtrBlock;
    friend class VoidPtrIterator;
    friend class VoidPtrPoolIterator;
    friend class Space;

    unsigned long m_refCount {0};

protected:
    void* m_address {nullptr};
    size_t m_size {0};
    VoidPtr();
    VoidPtr(void* adr, size_t s);

public:
    static VoidPtrPool* s_pool;
    static Space* s_space;

    virtual ~VoidPtr();

    void* operator new (size_t);
    void* operator new (size_t size, Space* space);
    void operator delete (void* space);

    void Grab();
    void Release();
};

#endif // VOIDPTR_H
