#pragma once

//Add 1 cycle if page boundary is crossed
enum class Instructions : Byte {

    //Various :
    BRK,    //Force break (see doc)
    NOP,    //No operation
    RTI,    //Return from Interrupt
    RTS,    //Return from Subroutine

    ADC,    //Add memory to Accumulator with Carry

    //Comparison :
    AND,    //AND memory with accumulator
    EOR,    //XOR Memory with Accumulator
    ORA,    //Or Memory with Accumulator
    BIT,    //Test bits in Memory with Accumulator (see doc)


    CMP,    //Compare memory with Accumulator
    CPX,    //Compare Memory and Index X
    CPY,    //Compare Memory and Index Y

    //Bit operations :
    ASL,    //Shift left one bit (Memory or Accumulator)
    LSR,    //Shift right one bit (Memory or Accumulator)
    ROL,    //Rotate one bit left (Memory or Accumulator)
    ROR,    //Rotate One Bit Right (Memory or Accumulator)

    //Branching :
    //Add 1 cycle if branch occurs on same page
    //Add 2 cycles if branch occurs on different page
    BCC,    //Branch on Carry = 0 (clear)
    BCS,    //Branch on Carry = 1 (Set)
    BEQ,    //Branch on result Zero
    BMI,    //Branch on N = 1 (result is Minus)
    BPL,    //Branch on N = 0 (result is positive)
    BNE,    //Branch on Z = 0 (result is Not zero)
    BVC,    //Branch on V = 0 (Overflow clear)
    BVS,    //Branch on V = 1 (Overflow set)

    //Flags manipulation : 
    CLC,    //Clear Carry Flag (C = 0)
    SEC,    //Set Carry Flag (C = 1)
    CLD,    //Clear decimal mode (D = 0)
    SED,    //Set decimal mode (D = 1)
    CLI,    //Clear Interrupt Disable Bit (I = 0)
    SEI,    //Set Interrupt Disable (I = 1)

    //Memory and indexes manipulation :
    INC,    //Increment Memory by one
    INX,    //Increment Index X by one
    INY,    //Increment Index Y by one

    DEC,    //Decrement Memory by one
    DEX,    //Decrement Index X by one
    DEY,    //Decrement Index Y by one

    //Jumps :
    JMP,    //Jump to new location 
    JSR,    //Jump to new location saving return address


    //Data transfer
    LDA,    //Load Accumulator with Memory
    LDX,    //Load Index X with Memory
    LDY,    //Load Index Y with Memory

    STA,    //Store Accumulator in Memory
    STX,    //Store Index X in Memory
    STY,    //Store Index Y in Memory

    TAX,    //Store Accumulator to Index X
    TAY,    //Transfer Accumulator to Index Y
    TSX,    //Transfer Stack Pointer to Index X
    TXA,    //Transfer Index X to Accumulator
    TYA,    //Transfer Index Y to Accumulator

    PHA,    //Push Accumulator on Stack
    PHP,    //Push processor Status on Stack
    PLA,    //Pull Accumulator from Stack
    PLP,    //Pull processor status from stack

    //Substraction :
    SBC,    //Substract Memory from Accumulator with Borrow

  

}