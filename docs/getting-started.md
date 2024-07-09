# Getting Started

## Setting Up Your Development Environment

Once you have decided to learn the 6502 assembly language you will need to set up your development environment. Below are the tools that you will need to get started:

### Text Editor

Any text editor will work for writing assembly code. In this course, we will use Visual Studio Code, but you can use any text editor that you are comfortable with.

### Assembler

Due to the simplicity of the 6502 many assemblers are avaliable in this course we will use the [Vasm](http://sun.hasenbraten.de/vasm/) assembler. any assembler will work but you may need to adjust the syntax of the code to match the assembler you are using.

### Emulator

To run your 6502 assembly code you will need an emulator that can run 6502 machine code. In this course, we will be using the 6502 emulator that I have created. You can find the emulator [here](https://github.com/mini51/6502emulator).

### Documentation

Due to the age of the 6502 microprocessor, there are many resources available to learn about it. In this course I will provide you with most of the information you need to get started but you may need to refer to the [6502.org](http://6502.org/) website for more information.

## Basic Concepts

Before we start writing code we need to understand some basic concepts of all microprocessors. Below are some of the key concepts that you need to understand:

### Registers

Registers are small circuits that store data temporarily and are volatile. They are used to store data that will be used by the microprocessoer to perorm operations and calulations. The 6502 microprocessor has 3 general purpose registers and 3 special purpose registers.

#### General Purpose Registers

- **A Register**: The Accumulator register is used to store the result of arithmetic and logical operations.

- **X Register**: The X register is used as a counter or an index in loops and arrays.

- **Y Register**: The Y register is used as a counter or an index in loops and arrays.

#### Special Purpose Registers

- **Program Counter (PC)**: The Program Counter register stores the address of the next instruction to be executed.

- **Stack Pointer (SP)**: The Stack Pointer register stores the address of the current location on the stack.

- **Status Register (SR)**: The Status Register stores the status of the microprocessor. It contains the flags that are set or cleared based on the result of the last operation.

### Control Unit

The control unit is the part of the microprocessor which is handling instructions. It is responsible for fetchin instructions from memory, decoding them, and executing them.

### Arithmetic and Logic Unit

The Arithmetic and Logic Unit (ALU) is the part of the microprocessor that performs arithmetic and logical operations as the name suggests. It is responsible for performing operations such as addition, subtraction, multiplication, division, and logical operations such as AND, OR, and NOT.

### Memory

Memory is technically not part of the microprocessor but it is an essential part of the microprocessor. Memory is used to store data and instructions that the microprocessor will use to perform operations. The 6502 microprocessor has a 16-bit address bus which means it can address up to 64KB of memory. The memory is typically divided into 3 sections for the 6502 microprocessor which are:

- **Zero Page** (0x0000 - 0x00FF) - The zero page is the first 256 bytes of memory and is used when you need to store data that will be accessed frequently and quickly.

- **Stack** (0x0100 - 0x01FF) - The stack is used to store data temporarily and is used to store the return address of subroutines.

- **RAM** (0x0200 - 0xFFFF) - The RAM is used to store data and instructions that the microprocessor will use to perform operations. The ram is slower to access than the zero page but can store more data.

- **ROM** (0xFFFA - 0xFFFF) - The ROM is used to store the reset vector and the interrupt vector.

Another important section that depends on the usage of the microprocessor is the **I/O** section. This section is used to communicate with other devices such as the keyboard, screen, and other peripherals. The I/O section we will be using is the way that Ben Eater has set up his 6502 computer. The I/O section is from 0x6000 to 0x7FFF.

## Binary and Hexadecimal

Binary and hexadecimal are the two number systems that are widley used computer science. Below is a brief over of each system:

### Binary

Binary is a base-2 number system which means that it ony has 2 digits which are 1 and 0. The reason why binary is heavily used in computer science is because you can represent it using eletrical signals which are either on or off. When a postion is on already and your trying to add 1 to it, it will overflow to the next postion. Below is an example of binary numbers:

```
0000 0001 - 1 in decimal
0000 0010 - 2 in decimal
0000 0011 - 3 in decimal
0000 0100 - 4 in decimal
0000 0101 - 5 in decimal
0000 0110 - 6 in decimal
0000 0111 - 7 in decimal
0000 1000 - 8 in decimal
0000 1001 - 9 in decimal
0000 1010 - 10 in decimal
...
```

### Hexadecimal

Hexadecimal is a base-16 number system which means that it has 16 digits which are 0-9 and A-F. Hexadecimal is used in computer science because it is easier to read and write than binary. When a position reaches 16 it will overflow to the next position. Below is an example of hexadecimal numbers:

```
00 - 0 in decimal
01 - 1 in decimal
02 - 2 in decimal
03 - 3 in decimal
04 - 4 in decimal
05 - 5 in decimal
06 - 6 in decimal
07 - 7 in decimal
08 - 8 in decimal
09 - 9 in decimal
0A - 10 in decimal
0B - 11 in decimal
0C - 12 in decimal
0D - 13 in decimal
0E - 14 in decimal
0F - 15 in decimal
10 - 16 in decimal
...
```

## [Next Section: Introduction to 6502 Assembly Language ➡️](introduction-to-6502-assembly-language.md)

<hr>

**If you have any questions, comments, or concerns, please feel free to reach out to me in the issues page** ❤️
