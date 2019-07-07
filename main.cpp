#include <QCoreApplication>
#include <memory>
#include <QDebug>
#include <iostream>

#include "smallallocator.h"

template<typename Type, int Size>
void printArr(Type const(& array)[Size])
{
    for (int i = 0; i < Size; i++)
    {
        std::cout << array[i] << std::endl;
    }
}

template<typename Type>
void printArr(Type const* array, size_t Size)
{
    for (size_t i = 0; i < Size; i++)
    {
        std::cout << array[i] << std::endl;
    }
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // https://stepik.org/lesson/12574/step/7?unit=3002


    /*
    SmallAllocator at1;
    const unsigned int arr1s = 3;
    const unsigned int arr2s = 5;
    const unsigned int arr3s = 7;

    auto arr1 = (int*) at1.Alloc(arr1s);
    auto arr2 = (int*) at1.Alloc(arr2s);
    auto arr3 = (int*) at1.Alloc(arr3s);
    for (unsigned int i = 0; i < arr1s; ++i)
    {
        arr1[i] = i;
    }
    for (unsigned int i = 0; i < arr2s; ++i)
    {
        arr2[i] = i;
    }
    for (unsigned int i = 0; i < arr3s; ++i)
    {
        arr3[i] = i;
    }

    std:: cout << "arr1:" << std::endl;
    printArr(arr1, arr1s);
    std:: cout << "arr2:" << std::endl;
    printArr(arr2, arr2s);
    std:: cout << "arr3:" << std::endl;
    printArr(arr3, arr2s);
    at1.Free(arr1);
    std:: cout << "arr1 after Free:" << std::endl;
    printArr(arr1, arr1s);
    at1.Free(arr2);

    arr1 = (int*) at1.ReAlloc(arr1, arr1s + 3);
    arr2 = (int*) at1.ReAlloc(arr2, arr2s + 5);
    printArr(arr1, arr2s);
    printArr(arr2, arr2s);

    */


    SmallAllocator A1;



    /// Mine
    ///=================================================================================================================

    const auto mp1ints = 10;
    const auto mp1s = sizeof(int) * mp1ints;
    auto mp1 = (int*) A1.Alloc(mp1s);
    assert(A1.getSpaceSize(mp1) == mp1s);
    for (size_t i = 0; i < mp1ints; ++i)
    {
        mp1[i] = i + 1;
    };
    A1.printAllBlocks();
    A1.Free(mp1);
    A1.printAllBlocks();
    mp1 = (int*) A1.Alloc(mp1s / 2);
    assert(A1.getSpaceSize(mp1) == mp1s / 2);
    for (size_t i = 0; i < mp1ints / 2; ++i)
    {
        mp1[i] = i + 2;
    };

    A1.printAllBlocks();
    mp1 = (int*) A1.ReAlloc(mp1, mp1s * 2);
    assert(A1.getSpaceSize(mp1) == mp1s * 2);
    A1.printAllBlocks();

    for (size_t i = 0; i < mp1ints * 2; ++i)
    {
        mp1[i] = i + 1;
    };
    A1.printAllBlocks();
    A1.Free(mp1);
    A1.printAllBlocks();



    mp1 = (int*) A1.Alloc(2 * sizeof(int));
    for (auto i = 0; i < 2; ++i)
    {
        mp1[i] = i + 1;
    }
    A1.printAllBlocks();
    A1.Free(mp1);
    A1.printAllBlocks();


    SmallAllocator sma2;

    auto ptr1 = (char*) sma2.Alloc(20);
    for (size_t i = 0; i < sma2.getSpaceSize(ptr1); ++i)
    {
        ptr1[i] = char(i) + 1;
    }
    sma2.printAllBlocks();

    auto ptr2 = (char*) sma2.Alloc(5);
    ptr1 = (char*) sma2.ReAlloc(ptr1, 25);
    sma2.printAllBlocks();
    sma2.Free(ptr1);
    sma2.printAllBlocks();
    for (size_t i = 0; i < sma2.getSpaceSize(ptr2); ++i)
    {
        ptr2[i] = char(i) + 1;
    }
    sma2.printAllBlocks();

    auto ptr3 = (char*) sma2.Alloc(5);
    sma2.printAllBlocks();
    for (size_t i = 0; i < sma2.getSpaceSize(ptr3); ++i)
    {
        ptr3[i] = char(i) + 1;
    }
    sma2.printAllBlocks();

    sma2.Free(ptr2);
    sma2.Free(ptr3);
    sma2.printAllBlocks();



    SmallAllocator sma3;
    auto p1 = (char*) sma3.Alloc(5);
    auto p2 = (char*) sma3.Alloc(5);
    auto p3 = (char*) sma3.Alloc(5);
    auto p5 = (char*) sma3.Alloc(5);
    sma3.printAllBlocks();
    sma3.Free(p1);
    sma3.Free(p2);
    sma3.Free(p3);
    sma3.printAllBlocks();
    auto p4 = (char*) sma3.Alloc(20);
    for (size_t i = 0; i < sma3.getSpaceSize(p4); ++i)
    {
        p4[i] = i + 1;
    }
    sma3.printAllBlocks();
    sma3.Free(p4);
    sma3.Free(p5);

    ///=================================================================================================================
    ///




    int* A1_P1 = (int*) A1.Alloc(sizeof(int));
    A1_P1 = (int*) A1.ReAlloc(A1_P1, 2 * sizeof(int));
    A1.Free(A1_P1);
    SmallAllocator A2;
    int* A2_P1 = (int*) A2.Alloc(10 * sizeof(int));
    for (unsigned int i = 0; i < 10; i++)
    {
        A2_P1[i] = i;
    }
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P1[i] != i)
        {
            std::cout << "ERROR 1" << std::endl;
        }
    int* A2_P2 = (int*) A2.Alloc(10 * sizeof(int));
    for (unsigned int i = 0; i < 10; i++)
    {
        A2_P2[i] = -1;
    }
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P1[i] != i)
        {
            std::cout << "ERROR 2" << std::endl;
        }
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P2[i] != -1)
        {
            std::cout << "ERROR 3" << std::endl;
        }
    A2_P1 = (int*) A2.ReAlloc(A2_P1, 20 * sizeof(int));
    for (unsigned int i = 10; i < 20; i++)
    {
        A2_P1[i] = i;
    }
    for (unsigned int i = 0; i < 20; i++)
        if (A2_P1[i] != i)
        {
            std::cout << "ERROR 4" << std::endl;
        }
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P2[i] != -1)
        {
            std::cout << "ERROR 5" << std::endl;
        }
    A2_P1 = (int*) A2.ReAlloc(A2_P1, 5 * sizeof(int));
    for (unsigned int i = 0; i < 5; i++)
        if (A2_P1[i] != i)
        {
            std::cout << "ERROR 6" << std::endl;
        }
    for (unsigned int i = 0; i < 10; i++)
        if (A2_P2[i] != -1)
        {
            std::cout << "ERROR 7" << std::endl;
        }
    A2.Free(A2_P1);
    A2.Free(A2_P2);



    //    /// Mine
    //    SmallAllocator ma1;

    //    const auto marr1s = 10;
    //    auto marr1 = ma1.Alloc(marr1s);
    //    auto marr2 = ma1.Alloc(5);

    //    ma1.Free(marr1);
    //    ma1.Free(marr2);



    /*
    char c = 255;
    qDebug() << int(c);
    typedef struct
    {
        int a;
        int b;
    } coord;
    */

    std::cout << "ALL DONE" << std::endl;

    return app.exec();
}
