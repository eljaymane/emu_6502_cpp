#pragma once
#include <types.h>

enum class AddressMode : Byte {

        ILL,    //Illegal Address mode

        ABS,    //Absolute
        ABX,    //Absolute, offset by X
        ABY,    //Absolute, offset by Y
        ACC,    //Accumulator (Implied)
        
        IMM,    //Immediate
        IMP,    //Implied

        IND,    //Indirect
        INX,    //Indexed Indirect using X (Pre-Indirect)
        INY,    //Indexed Indirect using Y (Post-Indirect)

        REL,    //Relative (Branch)

        ZPG,    //Zero-Page
        ZPX,    //Zero-page, offset by X
        ZPY     //Zero-page, offset by Y

};