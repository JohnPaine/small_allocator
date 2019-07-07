#ifndef HANDLE_H
#define HANDLE_H

#include "masterpointer.h"

template <typename Type>
class Handle
{
    MasterPointer<Type>* m_pointer {nullptr};

public:
    Handle() : m_pointer(new MasterPointer<Type>) {
        m_pointer->Grab();
    }
    Handle(const Handle<Type>& other) : m_pointer(other.m_pointer) {
        m_pointer->Grab();
    }
    Handle<Type>& operator=(const Handle<Type>& other) {
        if (this == &other || m_pointer == other.m_pointer) {
            return *this;
        }
        m_pointer->Release();
        other.m_pointer->Grab();
        return *this;
    }
    MasterPointer<Type>& operator->() {
        return *m_pointer;
    }
};

#endif // HANDLE_H
