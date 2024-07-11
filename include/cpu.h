#pragma once
#include <types.h>
#include <flags.h>
#include <address_modes.h>
#include <io_device.h>


class CPU : public IODevice{

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

                //Data bus (Memory)
                io_ptr m_Bus;


                // Represents the current processor status as an 8-bit bitfield value.
                Status m_CpuStatus;

                //Operand value (Implicit)
                Byte m_OpValue = 0;

                //IODevice Implementation

                inline Byte ReadByte(const address& addr) const override;
                inline Word ReadWord(const address& addr) const override;
                inline void WriteByte(const address& addr, const Byte byte) override;
                inline void WriteWord(const address& addr, const Word word) override;
                inline void WriteBytes(const address& addr, const vector<Byte>& bytes)override;


                inline void Write(const address& addr, const Byte data) { WriteByte( addr, data ); };
                inline void Write(const address& addr, const Word data) { WriteWord( addr, data ); };
                inline void Write(const address& addr, const vector<Byte>& data) { WriteBytes( addr, data ); };

                inline void SetImplicit(const Byte value){
                    m_OpValue = value;
                }
        
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

            public : //Address modes : https://www.masswerk.at/6502/6502_instruction_set.html#modes

                //Executes the right address mode and spits the cycle count to which we have a pointer (out var)
                virtual address ExecuteAddressing(const AddressMode addrMode, Byte& cycles);

            protected : 

                //These instructions have register A (the accumulator) as the target. 
                //Examples are LSR A and ROL A.
                virtual address Addr_Acc( Byte& cycles);

                //Implied Adressing : (No adressing needed)
                //These instructions act directly on one or more registers or flags
                //internal to the CPU. Therefor, these instructions are principally 
                //single-byte instructions, lacking an explicit operand. The operand 
                //is implied, as it is already provided by the very instruction.
                //Example : CLC clear the carry flag
                virtual address Addr_Imp( Byte& cycles );

                
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
                //
                //Examples : LDA $80,X load the contents of address "$0080 + X" into A
                //LDX $60,Y load the contents of address "$0060 + Y" into X
                virtual address Addr_ZeroX( Byte& cycles );
                virtual address Addr_ZeroY( Byte& cycles );

                //Indirect Addressing :
                //This mode looks up a given address and uses the contents of this address and the next one 
                //(in LLHH little-endian order) as the effective address.
                //In its basic form, this mode is available for the JMP instruction only. 
                //(Its generally use is jump vectors and jump tables.)
                //Generally, indirect addressing is denoted by putting the lookup address in parenthesis.
                //
                //Example : JMP ($FF82) jump to address given in addresses "$FF82" and "$FF83"
                virtual address Addr_Ind( Byte& cycles );

                //Pre-Indirect Addressing (Value at X is added to the provided addr):
                //Pre-indexed indirect address mode is only available in combination with the X-register.
                //It works much like the "zero-page,X" mode, but, after the X-register has been added to the base address,
                //instead of directly accessing this, an additional lookup is performed,
                //reading the contents of resulting address and the next one (in LLHH little-endian order), in order to determine the effective address.
                //
                //Example : LDA ($70,X) load the contents of the location given in addresses "$0070+X" and "$0070+1+X" into A (See doc for better understanding)
                virtual address Addr_PreInd( Byte& cycles );

                //Post-Indirect Addressing (Value at Y is added to the indirect lookup)
                //As indicated by the indexing term ",Y" being appended to the outside 
                //of the parenthesis indicating the indirect lookup,here, a pointer is first read (from the given zero-page address) 
                //and resolved and only then the contents of the Y-register is added to this to give the effective address.
                //
                //Example : LDA ($70),Y add the contents of the Y-register to the pointer provided in "$0070" and "$0071" and load the contents of this address into A
                virtual address Addr_PostInd( Byte& cycles );

                //Relative Addressing : (Conditional Branching)
                //This final address mode is exlusive to conditional branch instructions,
                //which branch in the execution path depending on the state of a given CPU flag.
                //Here, the instruction provides only a relative offset, which is added 
                //to the contents of the program counter (PC) as it points to the immediate next instruction.
                //The relative offset is a signed single byte value in two's complement encoding (giving a range of −128…+127),
                //which allows for branching up to half a page forwards and backwards.
                //
                //Example : BEQ $1005 branch to location "$1005", if the zero flag is set. if the current address is $1000, this will give an offset of $03.
                // Note: Because the PC is incremented by 2 during these instructions,
		        // the effective range must be within -126 to +129 bytes of the
		        // initial branch instruction.
                virtual address Addr_Branch( Byte& cycles );
                


};