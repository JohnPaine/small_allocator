#ifndef SPACE_H
#define SPACE_H

#include <cstddef>

#define SPACE_SIZE 512
#define HALF_SPACE_SIZE SPACE_SIZE / 2

class HalfSpace
{
    unsigned long m_nextByte {0};
    unsigned char m_bytes[HALF_SPACE_SIZE];

public:
    HalfSpace() = default;

    void* Allocate(size_t size);
    void Reinitialize();
};

class Space
{
    HalfSpace A, B;
    HalfSpace* m_active {nullptr};
    HalfSpace* m_inactive {nullptr};

    void Swap();

public:
    Space();

    void* Allocate(size_t size) noexcept(false);
};



#endif // SPACE_H
