#pragma once

#include <types.h>

union Status {
            struct {
                // Carry Flag:
				// The carry flag is set to 1 if the last operation caused an overflow
				// fom bit 7 of the result, or an underflow from bit 0.
				// Can be manually set with 'SEC', or cleared with 'CLC'
                bool C : 1;

                // Zero Flag:
				// Will be 1 if the last operation resulted in 0.
                bool Z : 1;

                // Interrupt Disable:
				// If set to 1, the processor will not respond to interrupts.
				// Can be enabled with 'SEI', or cleared with 'CLI'.
                bool I : 1;

                // Decimal Mode:
				// While enabled, the processor will use Binary Coded Decimal (BCD)
				// arithmetic.
				// Can be enabled with 'SED', or cleared with 'CLD'.

                bool D : 1;

                //Break Command:
                // Will be 1 when a 'BRK' instruction has been executed and an
				// interrupt has been generated.
				bool B : 1;

                // Unused:
				// This value is not technically used in the processor.
				bool U : 1;

                // Overflow Flag:
				// Will be set if an arithmetic operations resulted in a byte-overflow.
				// Ie. The number rolled over and became negative.
				bool V : 1;

                // Negative Flag:
				// Will be set if the last result was a negative value.
				// Determined by bit 7 being 1.
				bool N : 1;
            };

            Byte m_Value;

            Status() = default;
            Status(int v) : m_Value(v) {}
            Status(bool c, bool z, bool i, bool d, bool b, bool u, bool v, bool n)
                : C(c), Z(z), I(i), D(d), B(b), U(u), V(v), N(n) {}
        };

        enum class StatusFlag : Byte {
            CARRY           = (1 << 0), //Carry flag
            ZERO            = (1 << 1), //Zero flag
            INTERRUPT       = (1 << 2), //Interrupt disable
            DECIMAL         = (1 << 3), //Decimal Mode
            BREAK           = (1 << 4), //Break
            UNUSED          = (1 << 5), //Ignored (un-used)
            INT_OVERFLOW    = (1 << 6), //Overflow flag
            NEGATIVE        = (1 << 7)  //Negative flag
        };
