#include "cpu.h"
#include "utils.h"

    address CPU::ExecuteAddressing(const AddressMode addrMode, Byte& cycles) { 


        switch (addrMode) {
            case AddressMode::ILL:
                std:cerr << "Attempting to execute illegal access mode" << std::endl;
                break;
                 
            case AddressMode::ABS:
                return Addr_Abs(cycles);
            case AddressMode::ABX:
                return Addr_Abx(cycles);
            case AddressMode::ABY:
                return Addr_Aby(cycles);
            case AddressMode::IMM:
                return Addr_Imm(cycles);
            case AddressMode::IMP:
                return Addr_Imp(cycles);
            case AddressMode::IND:
                return Addr_Ind(cycles);
            case AddressMode::INX:
                return Addr_PreInd(cycles);
            case AddressMode::INY:
                return Addr_PostInd(cycles);
            case AddressMode::REL:
                return Addr_Branch(cycles);
            case AddressMode::ZPG:
                return Addr_Zero(cycles);
            case AddressMode::ZPX:
                return Addr_ZeroX(cycles);
            case AddressMode::ZPY:
                return Addr_ZeroY(cycles);
            
            default:
                std::cerr << "Error : Unimplemented addressing mode !" << std::endl;
               
        }
        return 0;
    }

    //Implied
    address CPU::Addr_Imp( Byte& cycles){
        SetImplicit(m_Acc); //Set the implicit value from the Accumulator

        cycles = 1;

        return m_PC;
    }

    //Immediate
    address CPU::Addr_Imm( Byte& cycles ) {
        const address addr = { m_PC++ };

        cycles = 1;
        return addr;
    }

    //Absolute
    address CPU::Addr_Abs ( Byte& cycles ) {
        const Byte addr_lo = ReadByte(m_PC);
        m_PC++;
        const Byte addr_hi = ReadByte(m_PC);
        m_PC++;
        cycles = 3;
        const address addr = MAKE_WORD(addr_hi,addr_lo);
        return addr;
    }

    //Zero-Page
    address CPU::Addr_Zero( Byte& cycles ) {
        const Byte addr_lo = ReadByte(m_PC);
        m_PC++;
        //addr_hi (page) = 0
        const address addr = MAKE_WORD(0,addr_lo);
        cycles =2;
        return addr;
    }

    //Indirect (Basic form : lookup)
    address CPU::Addr_Ind( Byte& cycles ) {
        const Byte addr_lo = ReadByte( m_PC );
        m_PC++;
        const Byte addr_hi = ReadByte( m_PC );
        m_PC++;
        const address lookup = {addr_hi,addr_lo} ;
        const address addr = ReadWord(lookup);
        cycles = 5;
        
        return addr;
    }

    //Pre-Indexed Indirect (Zero-Page,X)
    address CPU::Addr_PreInd( Byte& cycles ) {
        const Byte addr_lo = ReadByte(m_PC);
        cycles = 5;
        const address lookup = address::AddZeroPage(m_X,addr_lo,cycles);
        //cycles becomes 6 if we wrap arround the zero page
        return lookup;
    }
    
    //Post-Indexed Indirect (Zero-Page Y)
    address CPU::Addr_PostInd( Byte& cycles ) {
        Byte addr_lo = ReadByte(m_PC);
        m_PC++;
        address operand = {0x00,addr_lo};
        m_PC = ReadWord(operand);
        const address lookup = ReadWord(m_PC);
        cycles = 5;
        address addr = address::AddZeroPage(m_Y,lookup,cycles);
        //cycles becomes 6 if we wrap arround the zero page
        return addr;
    }

    //Relative Addressing (Conditional Branching)
    address CPU::Addr_Branch( Byte & cycles ){
        Byte rel = ReadByte(m_PC);
        m_PC++;
        if (rel & 0x80)
			rel |= 0xFF00;

        cycles = 1;
         return {rel};

    }



    