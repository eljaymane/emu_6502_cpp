#pragma once

#include <iostream>
#include <iomanip>

using Byte = unsigned char;     //8 bits
using Word = unsigned short;    //16 bits
using namespace std;

union address {
    private :
        Word value;
        struct {
                Byte record;
                Byte page;
        };

    address() = default;
    address(int val) : value(val) {};
    address(Byte page, Byte record) : record(record), page(page) {};
    
    friend std::ostream& operator<<( std:: ostream& os, const address& addr ) {
        os << hex
                << uppercase
                << setfill('0')
                << setw(4)
                << addr.value;
    }
};