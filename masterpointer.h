#ifndef MASTERPOINTER_H
#define MASTERPOINTER_H

#include "voidptr.h"

template <typename Type>
class MasterPointer : public VoidPtr
{
public:
    MasterPointer() : VoidPtr(new Type, sizeof(Type)) {}
    MasterPointer(const MasterPointer<Type>&) = delete;
    MasterPointer<Type>& operator =(const MasterPointer<Type>&) = delete;

    virtual ~MasterPointer() {
        ((Type*)m_address)->Type::~Type();
    }

    Type* operator ->() {
        return (Type*)m_address;
    }
};

template <typename Type>
class SpaceMasterPointer : public VoidPtr
{
public:
    SpaceMasterPointer() : VoidPtr(new (s_space) Type, sizeof(Type)) {}
    SpaceMasterPointer(const SpaceMasterPointer<Type>&) = delete;
    SpaceMasterPointer<Type>& operator =(const SpaceMasterPointer<Type>&) = delete;

    virtual ~SpaceMasterPointer() {
        ((Type*)m_address)->Type::~Type();
    }
    Type* operator ->() {
        return (Type*)m_address;
    }
};

#endif // MASTERPOINTER_H
