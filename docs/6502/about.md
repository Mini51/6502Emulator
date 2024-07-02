# What is the 6502?

The 6502 is a simple microprocessor that was created in 1975 by MOS Technology. This microprocessor was a major player in home computing during the 1980's. You may have heard of some of the computers and game consoles that used the 6502, some of these include but are not limited too: Apple I, Apple II, Commodore PET, Commodore VIC-20, Commodore 64, Nintendo Entertainment System (NES), aswell as the Atari 2600. The 6502 was a very popular microprocessor due to its low cost and high performance. The 6502 was also used in many other systems which needed a low cost microprocessor.

## Registers

### Defintion

Registers are a piece of memory that is in the CPU itself and is not apart of the main memory. The purpouse of these registers is to store data that the CPU needs to perform operatoin on.

### General Purpose Registers

#### Accumulator

The accumulator is an 8-bit register which stores the result of mathmatical operations that are performed by the 6502. The accumulator is also used to store data that is used by the 6502.

#### X Register

The x register is an 8-bit register which is used to store data that is used by the 6502. The x register is also used to store the offset for indexed addressing modes.

#### Y Register

The y register is an 8-bit register which is used to store data that is used by the 6502. The y register is also used to store the offset for indexed addressing modes.

### Control Registers

#### Program Counter

The program counter is a 16-bit register which stores the address of the next instruction that the 6502 will execute.

#### Stack Pointer

The stack pointer is an 8-bit register which stores the address of the current location on the stack in memory.

#### Status Register

The status register is an 8-bit register which stores the status of the 6502. The status register is made up of 8 flags which are used to store the status of the 6502. The flags are as follows:

```
n v u b d i z c
0 0 0 0 0 0 0 0

N - Negative ( Used to show whether the result of the last operation was negative )
V - Overflow ( Used to show whether the result of the last operation caused an overflow )
U - Unused ( Always 1, not used by the 6502 )
B - Break ( Used to show whether the break command was executed )
D - Decimal Mode ( Used to show whether the decimal mode is enabled )
I - Interrupt Disable ( Used to show whether the interrupt disable is enabled )
Z - Zero ( Used to show whether the result of the last operation was zero )
C -Carry ( Used to show whether the last operation caused a carry )
```

## Busses

### Definition

A bus is a group of wires that are used to transfer data around differnet parts of a CPU. The 6502 has 2 busses, the data bus and the address bus.

### Data Bus

The data bus is an 8-bit bus which is used to transfer data around the CPU and to and from memory.

### Address Bus

The address bus is a 16-bit bus which is used to interact with memory. The address bus is used to read and write data to mermoy at a specific address.

## Control Circuit

### Definition

The control circuit is a part of all CPUs that is to control the flow of data, the logic of the CPU, and the timing of the CPU.

## Arithmetic Logic Unit (ALU)

### Definition

The Arithmetic Logic Unit (ALU) is a part of the CPU that is performs arithmatic and logical operations on data that is given to it.

### Operations

On the 6502, the ALU can perform the following operations:

- Add
- Subtract
- AND
- OR
- XOR
- Shift Left
- Shift Right
- Rotate Left
- Rotate Right
- Compare

# Next Chapter

Now that you know what the 6502 is and what is inside of it, you can move on to the next section which will explain how the 6502 workss in the next section [How does the 6502 work?](docs/6502/how-does-the-6502-work.md)

**P.S. If you are struggling to understand any of the concepts in this document, please refer to the [Glossary](docs/glossary.md) for an explanation of the terms used in this document.**
