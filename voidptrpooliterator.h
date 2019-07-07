#ifndef VOIDPTRPOOLITERATOR_H
#define VOIDPTRPOOLITERATOR_H

#include "voidptriterator.h"

class VoidPtrBlock;

class VoidPtrPoolIterator : public VoidPtrIterator
{
    VoidPtrBlock* m_block {nullptr};
    int m_slot {0};

    virtual void Advance();

public:
    virtual ~VoidPtrPoolIterator();
    VoidPtrPoolIterator(VoidPtrBlock* block);

    // VoidPtrIterator interface
    virtual bool More() const override;
    virtual VoidPtr* Next() override;
};

#endif // VOIDPTRPOOLITERATOR_H
