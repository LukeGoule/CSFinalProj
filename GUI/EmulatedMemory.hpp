#pragma once

#ifndef EMULATEDMEMORY_HPP
#define EMULATEDMEMORY_HPP

#include <stdexcept>
#include <string>
#include <iostream>

// Template class is defined within the header. This is not good
// but has to be done to allow compilation on G++. A solution to 
// this is to allow an argument to the constructor for the wanted
// memory. This can be done fast but won't be done since template
// classes are cooler.
template<unsigned long long MemSize> class EmulatedMemory
{

    uint8_t* m_pRealMemory;   // Memory!! Not a string!

public:

    inline EmulatedMemory()
        : m_pRealMemory(nullptr)
    {
        this->ResetMemory();
    }

    inline ~EmulatedMemory()
    {}

    inline void ResetMemory()
    {
        m_pRealMemory = (uint8_t*)malloc(MemSize);
    }

    inline bool CheckAddress(std::string OperationName, unsigned long long uAddress)
    {
        if (uAddress < 0 || uAddress > MemSize)
        {
            throw std::runtime_error{ "[" + OperationName + "] Attempted access outside of useable memory!" };
        }

        return true;
    }

    inline uint8_t Read(unsigned long long uAddress)
    {
        CheckAddress("Read", uAddress);

        return m_pRealMemory[uAddress];
    }

    inline void Write(unsigned long long uAddress, uint8_t btValue)
    {
        CheckAddress("Write", uAddress);

        m_pRealMemory[uAddress] = btValue;
    }
};

#endif