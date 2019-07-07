#include "voidptrpooliterator.h"
#include "voidptrpool.h"

void VoidPtrPoolIterator::Advance()
{
    while (m_block)
    {
        if (m_slot >= BLOCK_SIZE)
        {
            m_block = m_block->next;
            m_slot = 0;
        }
        else if (m_block->poolSlots[m_slot].m_size != 0)
        {
            break;
        }
        m_slot++;
    }
}

VoidPtrPoolIterator::~VoidPtrPoolIterator()
{
}

VoidPtrPoolIterator::VoidPtrPoolIterator(VoidPtrBlock* block) : m_block(block)
{
    Advance();
}

bool VoidPtrPoolIterator::More() const
{
    return m_block != nullptr;
}

VoidPtr* VoidPtrPoolIterator::Next()
{
    if (!m_block)
    {
        return nullptr;
    }

    auto vp = &m_block->poolSlots[m_slot];
    Advance();
    return vp;
}
