# 6502Emulator
This is an emulator for the 6502 microprocessor.

The emulator is written in C++ and has been tested with the [Klaus Dormann's 6502 functional test suite](github.com/Klaus2m5/6502_65C02_functional_tests/). The purpose of this emulator was to gain a better understanding of a CPU's functions as well as a way to learn C++. Due to this, the emulator is not optimized for speed and is not intended to be used in any production environment, but rather as a learning tool. Only legal opcodes are implemented due to the fact that illegal opcodes are highly unstable and can vary between different 6502 chips.

## Usage
To use this emulator library, you need to include the mos6502.h header file in your project and link the mos6502.cpp file.

Below are all the public functions that can be used to interact with the emulator.
```c++


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

for an example of how to use the emulator see the example.cpp file in the src directory.

