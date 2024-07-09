#pragma once

#include <vector>
#include <memory>
#include <types.h>

using namespace std;

class IODevice {

    public:

            virtual Byte ReadByte(const address& addr) const = 0;

            virtual Word ReadWord(const address& addr) const = 0;

            virtual void WriteByte(const address& addr, const Byte data) = 0;

            virtual void WriteWord(const address& addr, const Word data) = 0;

            virtual void WriteBytes(const address& addr, const vector<Byte>& bytes) = 0;

            virtual void Write(const address& addr, const Byte data) { WriteByte( addr, data ); }
            virtual void Write(const address& addr, const Word data) { WriteWord( addr, data ); }
            virtual void Write(const address& addr, const vector<Byte>& data) { WriteBytes( addr, data ); }



            using io_ptr = shared_ptr<IODevice>;
};