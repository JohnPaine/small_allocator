#ifndef VOIDPTRITERATOR_H
#define VOIDPTRITERATOR_H

class VoidPtr;

class VoidPtrIterator
{
protected:
    VoidPtrIterator() {}

public:
    virtual ~VoidPtrIterator() {}
    virtual bool More() const = 0;
    virtual VoidPtr* Next() = 0;
};

#endif // VOIDPTRITERATOR_H
