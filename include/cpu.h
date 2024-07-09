#pragma once
#include <types.h>
#include <flags.h>


class CPU {

            protected :

                // Program Counter:
                // 16-bit register pointing to the next instruction for execution.
                // This value is automatically incremented when an instruction is executed,
                // but may be modified manually for jumps or interupts.
                Word m_PC; 

                // Stack Pointer:
                // Holds the lower 8-bits of the next free location on the stack.
                // The stack is generally mapped to 0x0100 - 0x01FF on the and represents
                // 256 bytes of memory.
                // Pushing bytes to the stack will cause the SP to decrement.
                // Pulling bytes to the stack will increment the SP.
                Byte m_SP;

                // Accumulator:
                // 8-bits used in all math and logical operations.
                // Contents of which can be on either stack, or memory.
                Byte m_Acc;

                // Registers X, and Y:
                // 8-bit memory register.
                Byte m_X,m_Y;


                // Represents the current processor status as an 8-bit bitfield value.
                Status m_CpuStatus;
        
            public:

            //Registers & Status : 

                inline const Word GetProgramCounter() const { return m_PC; }

                inline const Byte GetStackPointer() const { return m_SP; }

                inline const Byte GetAccumulator() const { return m_Acc; }

                inline const Byte GetX() const { return m_X; }

                inline const Byte GetY() const { return m_Y; }

                inline const Status GetStatus() const { return m_CpuStatus; }

                // Masking with a statusFlag to extract the desired flag byte value
                inline const Byte GetStatusFlag( const StatusFlag statusFlag ) const {
                    return m_CpuStatus.m_Value & static_cast<Byte>(statusFlag) ? 1 : 0; 
                }

                inline bool HasStatusFlag( const StatusFlag statusFlag ) const {
                    return m_CpuStatus.m_Value & static_cast<Byte>(statusFlag);
                }

                inline void SetStatusFlag( const StatusFlag statusFlag, const bool val = true) {
                    if(val) m_CpuStatus.m_Value |= static_cast<Byte>(statusFlag);
                    //Switching all bits to 1 and desired bit to 0, then ANDing it with value to set it to :
                    // 0 if it's set to 1 and leave it at 0 if already set to it
                    else m_CpuStatus.m_Value &= ~(static_cast<Byte>(statusFlag)); 
                }

                void Reset(){
                    m_PC = 0xFFFC;
                    m_SP = 0x0100;
                    m_CpuStatus = {0};
                }

           
            protected : //Address modes : https://www.masswerk.at/6502/6502_instruction_set.html#modes

                //Implied Adressing : (No adressing needed)
                //These instructions act directly on one or more registers or flags
                //internal to the CPU. Therefor, these instructions are principally 
                //single-byte instructions, lacking an explicit operand. The operand 
                //is implied, as it is already provided by the very instruction.
                //Example : CLC clear the carry flag
                virtual address Addr_Impl( Byte& cycles );

                
                //Immediate Addressing : 
                //Here, a literal operand (8bit value) is given immediately after the instruction.
                //The total instruction length is always 2 bytes (instr code + operand)
                //In assembler, the mode is usually indicated by a "#" prefix adjacent to the operand.
                //Example : LDA #$07 load the literal hexidecimal value "$7" into the accumulator
                virtual address Addr_Imm( Byte& cycles );

                //Absolute addressing : 
                //provides the 16-bit address of a memory location,
                //the contents of which used as the operand to the instruction.
                //In machine language, the address is provided in two bytes immediately after the instruction
                //making these 3-byte instructions) in low-byte, high-byte order (LLHH) or little-endian.
                //In assembler, conventional numbers (HHLL order or big-endian words) are used to provide the address.
                //Examples : LDA $3010 load the contents of address "$3010" into the accumulator
                //JMP $4000 jump to (continue with) location "$4000"
                virtual address Addr_Abs( Byte& cycles );

                //Zero-Page Addressing :
                //The zero-page address mode is similar to absolute address mode, but
                //these instructions use only a single byte for the operand, the low-
                //byte, while the high-byte is assumed to be zero by definition. 
                //Therefore they take only two cycles (one less than Addr_Abs)
                //Example : LDA $80 load the contents of address "$0080" into the accumulator (note that $80 is assumed to be $0080)
                virtual address Addr_Zero( Byte& cycles );


                //Indexed Addressing : Absolute,X and Aboslute,Y 
                //Indexed addressing adds the contents of either the X-register or the 
                //Y-register to the provided address to give the effective address,
                //which provides the operand.
                //As the base address is a 16-bit value, these are generally 3-byte instructions.
                //Since there is an additional operation to perform to determine the effective address, 
                //these instructions are one cycle slower than those using absolute addressing mode.****
                //
                //Example : LDA $3120,X load the contents of address "$3120 + X" into A
                //LDX $8240,Y load the contents of address "$8240 + Y" into X
                //
                //**** If the addition of the contents of the index register effects in a change of the high-byte given by the base address 
                //So that the effective address is on the next memory page, the additional operation to increment the high-byte takes another CPU cycle. 
                //This is also known as a crossing of page boundaries.
                virtual address Addr_Abx( Byte& cycles );
                virtual address Addr_Aby( Byte& cycles );


                //Indexed Addressing : Zero-Page,X (and Zero-Page,Y)
                //As with absolute addressing, there is also a zero-page mode for indexed addressing.
                //However, this is generally only available with the X-register. 
                //(The only exception to this is LDX, which has an indexed zero-page mode utilizing the Y-register.)
                //
                //Unlike absolute indexed instructions with 16-bit base addresses, zero-page indexed instructions 
                //never affect the high-byte of the effective address, which will simply wrap around in the zero-page,
                //and there is no penalty for crossing any page boundaries.
                //Examples : LDA $80,X load the contents of address "$0080 + X" into A
                //LDX $60,Y load the contents of address "$0060 + Y" into X
                virtual address Addr_ZeroX( Byte& cycles );
                virtual address Addr_ZeroY( Byte& cycles );




};