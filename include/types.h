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
    public :

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

    static address AddZeroPage(Byte value, const Byte record, Byte& cycles){
        Byte new_record = record + value;
        if(new_record > 255) {
            new_record -= 255; //Wrapping up
            cycles++;
        }
        return address {0,new_record};
    }

    static address AddZeroPage(Byte value,const address& addr, Byte& cycles){
        return AddZeroPage(value,addr.record,cycles);
    }
};