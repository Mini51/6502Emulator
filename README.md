# 6502 Emulator

This project is an emulator for the 6502 microprocessor, developed in C++. It has been thoroughly tested using [Klaus Dormann's 6502 functional test suite](https://github.com/Klaus2m5/6502_65C02_functional_tests/). The primary aim of this emulator is to deepen understanding of CPU operations and to serve as a learning exercise for C++ programming. As such, the emulator is not optimized for speed and is not intended for production use. It supports only legal opcodes, as illegal opcodes can be unstable and vary between different 6502 chips.

## Purpose

This project aims to help students grasp the fundamentals of how computers operate. For those interested in learning more about the 6502 microprocessor, its workings, and how to write an emulator, please refer to our [Getting Started](docs/getting-started.md) guide.

# Basic Usage

To use this emulator library, you need to include the mos6502.h header file in your project and link the mos6502.cpp file.

Below are all the public functions that can be used to interact with the emulator.

```cpp

mos6502(); // constructor to initialize the emulator


getPC(); // Get the program counter
setPC(uint16_t data); // Set the program counter

getSP(); // Get the stack pointer
setSP(uint8_t data); // Set the stack pointer

getSR(); // Get the status register
setSR(uint8_t data); // Set the status register

getAC(); // Get the accumulator
setAC(uint8_t data); // Set the accumulator

getXR(); // Get the x register
setXR(uint8_t data); // Set the x register

getYR(); // Get the y register
setYR(uint8_t data); // Set the y register

setFlag(flag_bits flag, bool state); // Set a flag
getFlag(flag_bits flag); // Get a flag

readByte(uint16_t address); // Read a byte from memory
writeByte(uint16_t address, uint8_t byte); // Write a byte to memory

pushStack(uint8_t byte); // Push a byte to the stack
popStack(); // Pop a byte from the stack

loadMemory(std::vector<uint8_t> data); // Load memory into the emulator
dumpMemory(std::string localDir); // Dump memory to a file

step(); // Step through the program
reset(); // Reset the emulator
IRQ(); // Trigger an IRQ
NMI(); // Trigger an NMI

```
