# Your First program

With all of the information you have learned so far you are ready to write your first program. In this section of the guide we will write a simple program that will write hello world to the screen. If you have not already set up your development environment please refer to the [Getting Started](getting-started.md) section of the guide. With that out of the way let's get started.

## Understanding the assembly code, addressing modes

### Addressing Modes

The 6502 microprocessor has multiple different ways to accsess memory and data called addressing modes. Below are the addressing modes that are available for the 6502 microprocessor.

| Addressing Mode | Description                                                                                                                                | mnemonic    |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------ | ----------- |
| Absolute        | The address of the operand is specified in the instruction.                                                                                | OPC $LLHH   |
| Absolute X      | The address of the operand is specified in the instruction and the X register is added to it.                                              | OPC $LLHH,X |
| Absolute Y      | The address of the operand is specified in the instruction and the Y register is added to it.                                              | OPC $LLHH,Y |
| Accumulator     | The operand is in the accumulator register.                                                                                                | OPC A       |
| Immediate       | The operand is specified in the instruction.                                                                                               | OPC #$BB    |
| Implied         | The operand is implied by the instruction.                                                                                                 | OPC         |
| Indirect        | The address of the operand is specified in the instruction and is the address of the least significant byte of the address of the operand. | OPC ($LLHH) |
| Indirect X      | The address of the operand is specified in the instruction and the X register is added to it.                                              | OPC ($LL,X) |
| Indirect Y      | The address of the operand is specified in the instruction and the Y register is added to it.                                              | OPC ($LL),Y |
| Relative        | The address of the operand is specified in the instruction as a signed offset.                                                             | OPC $BB     |
| Zero Page       | The address of the operand is specified in the instruction and is the address of the operand in the zero page.                             | OPC $LL     |
| Zero Page X     | The address of the operand is specified in the instruction and the X register is added to it.                                              | OPC $LL,X   |
| Zero Page Y     | The address of the operand is specified in the instruction and the Y register is added to it.                                              | OPC $LL,Y   |

### Instruction Set for the 6502 microprocessor

The 6502 microprocessor has a limited instruction set that is easy to learn and consists of only 56 instructions. Below are a list of all the instructions that are available for the 6502 microprocessor and a brief description of what they do.

| Instruction | Description                 |
| ----------- | --------------------------- |
| ADC         | Add with Carry              |
| AND         | Logical AND                 |
| ASL         | Arithmetic Shift Left       |
| BCC         | Branch if Carry Clear       |
| BCS         | Branch if Carry Set         |
| BEQ         | Branch if Equal             |
| BIT         | Bit Test                    |
| BMI         | Branch if Minus             |
| BNE         | Branch if Not Equal         |
| BPL         | Branch if Positive          |
| BRK         | Break                       |
| BVC         | Branch if Overflow Clear    |
| BVS         | Branch if Overflow Set      |
| CLC         | Clear Carry                 |
| CLD         | Clear Decimal               |
| CLI         | Clear Interrupt Disable     |
| CLV         | Clear Overflow              |
| CMP         | Compare                     |
| CPX         | Compare X Register          |
| CPY         | Compare Y Register          |
| DEC         | Decrement                   |
| DEX         | Decrement X Register        |
| DEY         | Decrement Y Register        |
| EOR         | Exclusive OR                |
| INC         | Increment                   |
| INX         | Increment X Register        |
| INY         | Increment Y Register        |
| JMP         | Jump                        |
| JSR         | Jump to Subroutine          |
| LDA         | Load Accumulator            |
| LDX         | Load X Register             |
| LDY         | Load Y Register             |
| LSR         | Logical Shift Right         |
| NOP         | No Operation                |
| ORA         | Logical Inclusive OR        |
| PHA         | Push Accumulator            |
| PHP         | Push Processor Status       |
| PLA         | Pull Accumulator            |
| PLP         | Pull Processor Status       |
| ROL         | Rotate Left                 |
| ROR         | Rotate Right                |
| RTI         | Return from Interrupt       |
| RTS         | Return from Subroutine      |
| SBC         | Subtract with Carry         |
| SEC         | Set Carry                   |
| SED         | Set Decimal                 |
| SEI         | Set Interrupt Disable       |
| STA         | Store Accumulator           |
| STX         | Store X Register            |
| STY         | Store Y Register            |
| TAX         | Transfer Accumulator to X   |
| TAY         | Transfer Accumulator to Y   |
| TSX         | Transfer Stack Pointer to X |
| TXA         | Transfer X to Accumulator   |
| TXS         | Transfer X to Stack Pointer |
| TYA         | Transfer Y to Accumulator   |

For a more detailed explanation of each addressing mode and instruction please refer to the [6502 Instruction Set](https://www.masswerk.at/6502/6502_instruction_set.html) page.

## Writing the code

Now that you have a basic understanding of the 6502 microprocessor and the instruction set we can start writing our first program. Lets get started with something simple like transfering data around the registers and memory.

```asm
; First Program
; This program will transfer data all around the registers and memory

; First we will load the accumulator with some data
LDA #$41

; Next we will transfer the data from the accumulator to the X register
TAX

; Then we will transfer the data from the accumulator to the Y register
TAY

; Finally we will store the data in the Y register to the memory address $0200
STY $0200

; End of the program
BRK
```

As you can see in this program we are transfering data between registers and memory. Lets run this program and lets see what happens

## Running the program

First we must assemle the program using VASM and then we can run the program using the 6502 emulator. To assemble the program run the following command:

```bash
vasm6502 -Fbin -dotdir first-program.asm -o example.bin
```

Next we will put this in the same directory as our compiled emulator, if you have not compiled the example project emulator please refer to the [README](../README.md) file. Once you have done that you can run the following command to run the program:

```bash
./build/example6502
```

If everything went well you should start seeing the following things occur:

1. The accumulator will be loaded with the value 0x41
2. The X register will be loaded with the value 0x41
3. The Y register will be loaded with the value 0x41
4. The value 0x41 will be stored in the memory address 0x0200 (This can not be checked in the emulator but in the real hardware you would be able to see this)

## Conclusion

In this section we covered some of the basic concepts of the 6502 microprocessor and wrote our first program. In the next section we will cover some more advanced topics that will allow you to write more complex programs for the 6502 microprocessor.

## [Next Section: advanced-topics](advanced-topics.md)

**If you have any questions, comments, or concerns, please feel free to reach out to me in the issues page** ❤️
