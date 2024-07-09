#pragma once

#include <vector>
#include <types.h>
#include <io_device.h>
#include <utils.h>

using namespace std;


    //Originally the chip supported addressing for a max 64kb of memory 

    //Holds the memory for the 6502 chip
    


    class Memory : public IODevice {

        using memory = vector<Byte>;

        private :

            const size_t m_Size;
            memory m_Data;

        public :

            using iterator = memory::iterator;
            using const_iterator = memory::const_iterator;

            static io_ptr Make(const size_t size = MAX_MEMORY_KB);

            //Moving
            Memory(Memory&&) = default;
            Memory& operator=(const Memory&) = default;

            //No copying
            Memory(const Memory&) = delete;
            Memory& operator=(const Memory&) = delete;

            virtual inline void Clear(const Byte value) {
                std:fill(m_Data.begin(), m_Data.end(), value);
            }

            inline const size_t GetSize() const { return m_Size; }

            inline Byte* GetData() { return m_Data.data(); } //Current location of the vector pointer

            Byte& operator[](size_t i) { return m_Data[i]; }

    };
