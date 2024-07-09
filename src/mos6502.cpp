#include "../include/mos6502.h"

#pragma region Private functions

// Addressing modes
uint16_t mos6502::addressingACC()
{
    return 0x00;
};
uint16_t mos6502::addressingIMM()
{
    return programCounter++;
};
uint16_t mos6502::addressingABS()
{
    // Read low byte from memory
    uint8_t lowByte = readByte(programCounter);
    programCounter++;

    // Read high byte from memory
    uint8_t highByte = readByte(programCounter);
    programCounter++;

    // Combine bytes to form the address
    uint16_t address = (highByte << 8) | lowByte;

    return address;
}
uint16_t mos6502::addressingZER()
{
    uint16_t address = readByte(programCounter);

    programCounter++;

    return address;
};
uint16_t mos6502::addressingZEX()
{
    uint16_t baseAddress = readByte(programCounter);

    programCounter++;

    uint16_t address = (baseAddress + xRegister) & 0xFF;

    return address;
};
uint16_t mos6502::addressingZEY()
{
    uint16_t baseAddress = readByte(programCounter);

    programCounter++;

    uint16_t address = (baseAddress + yRegister) & 0xFF;

    return address;
};
uint16_t mos6502::addressingABX()
{
    uint16_t lowByte = readByte(programCounter);
    programCounter++;

    uint16_t highByte = readByte(programCounter);
    programCounter++;

    uint16_t address = (highByte << 8) | lowByte;

    address += xRegister;

    return address;
};
uint16_t mos6502::addressingABY()
{
    uint16_t lowByte = readByte(programCounter);
    programCounter++;

    uint16_t highByte = readByte(programCounter);
    programCounter++;

    uint16_t address = (highByte << 8) | lowByte;

    address += yRegister;

    return address;
};
uint16_t mos6502::addressingIMP()
{
    return 0;
};
uint16_t mos6502::addressingREL()
{
    int8_t offset = readByte(programCounter);

    programCounter++;

    uint16_t targetAddress = programCounter + offset;

    return targetAddress;
};
uint16_t mos6502::addressingINX()
{
    uint16_t baseAddress = readByte(programCounter);

    programCounter++;

    baseAddress += xRegister;

    baseAddress &= 0xFF;

    uint8_t lowByte = readByte(baseAddress);

    uint8_t highByte = readByte((baseAddress + 1) & 0xFF);

    uint16_t address = (highByte << 8) | lowByte;

    return address;
};
uint16_t mos6502::addressingINY()
{
    uint16_t baseAddress = readByte(programCounter);

    programCounter++;

    uint8_t lowByte = readByte(baseAddress);

    uint8_t highByte = readByte((baseAddress + 1) & 0xFF);

    uint16_t address = (highByte << 8) | lowByte;

    address += yRegister;

    return address;
};
uint16_t mos6502::addressingIND()
{

    uint16_t addrL;
    uint16_t addrH;
    uint16_t effL;
    uint16_t effH;
    uint16_t abs;
    uint16_t addr;

    addrL = readByte(programCounter);
    programCounter++;
    addrH = readByte(programCounter);
    programCounter++;

    abs = (addrH << 8) | addrL;

    effL = readByte(abs);
    effH = readByte(abs + 1);

    addr = effL + 0x100 * effH;

    return addr;
};
void mos6502::ADC(uint16_t address)
{

    // Get value and flags
    uint8_t value = readByte(address);
    bool carry = getFlag(CARRY_FLAG_BIT);
    bool decimalMode = getFlag(DECIMAL_FLAG_BIT);

    uint16_t result = accumulator + value + (carry ? 1 : 0);

    if (decimalMode)
    {
        // Perform BCD mode adjustment
        if (((accumulator & 0x0F) + (value & 0x0F) + (carry ? 1 : 0)) > 0x09)
        {
            result += 0x06; // Adjust for carry in the least significant nibble
        }
        if (result > 0x99)
        {
            result += 0x60; // Adjust for carry in the most significant digit
        }
    }

    // Update the flags
    setFlag(CARRY_FLAG_BIT, result > 0xFF);
    setFlag(ZERO_FLAG_BIT, !(result & 0xFF));
    setFlag(OVERFLOW_FLAG_BIT, ~((accumulator ^ value) & 0x80) & ((accumulator ^ result) & 0x80));
    setFlag(NEGATIVE_FLAG_BIT, result & 0x80);

    accumulator = result & 0xFF;
};
void mos6502::AND(uint16_t address)
{
    // Fetch the value from memory
    uint8_t value = readByte(address);

    // Perform the exclusive-OR operation
    accumulator &= value;

    // Update the zero flag
    setFlag(ZERO_FLAG_BIT, !accumulator);

    // Update the negative flag
    setFlag(NEGATIVE_FLAG_BIT,
            accumulator & 0x80);

    return;
};
void mos6502::ASL(uint16_t address)
{
    uint8_t value = readByte(address);
    setFlag(CARRY_FLAG_BIT, value & 0x80);
    value <<= 1;
    value &= 0xFF;

    setFlag(ZERO_FLAG_BIT, !value);
    setFlag(NEGATIVE_FLAG_BIT, value & 0x80);

    writeByte(address, value);
};
void mos6502::ASL_ACC(uint16_t address)
{

    uint8_t value = accumulator;

    setFlag(CARRY_FLAG_BIT, value & 0x80);
    value <<= 1;
    value &= 0xFF;

    setFlag(ZERO_FLAG_BIT, !value);
    setFlag(NEGATIVE_FLAG_BIT, value & 0x80);

    accumulator = value;

    return;
};
void mos6502::BCC(uint16_t address)
{
    if (!getFlag(CARRY_FLAG_BIT))
    {

        programCounter = address;
    }

    return;
};
void mos6502::BCS(uint16_t address)
{
    if (getFlag(CARRY_FLAG_BIT))
    {

        programCounter = address;
    }

    return;
};
void mos6502::BEQ(uint16_t address)
{
    if (getFlag(ZERO_FLAG_BIT))
    {

        programCounter = address;
    }

    return;
};
void mos6502::BIT(uint16_t address)
{

    uint8_t value = readByte(address);

    uint8_t result = accumulator & value;

    setFlag(NEGATIVE_FLAG_BIT, value & (1 << 7));
    setFlag(OVERFLOW_FLAG_BIT, value & (1 << 6));

    setFlag(ZERO_FLAG_BIT, !result);

    return;
};
void mos6502::BMI(uint16_t address)
{
    if (getFlag(NEGATIVE_FLAG_BIT))
    {

        programCounter = address;
    }

    return;
};
void mos6502::BNE(uint16_t address)
{
    if (!getFlag(ZERO_FLAG_BIT))
    {

        programCounter = address;
    }

    return;
};
void mos6502::BPL(uint16_t address)
{
    if (!getFlag(NEGATIVE_FLAG_BIT))
    {

        programCounter = address;
    }

    return;
};
void mos6502::BRK(uint16_t address)
{
    programCounter++;
    pushStack((programCounter >> 8) & 0xFF);
    pushStack(programCounter & 0xFF);
    pushStack(statusRegister | (1 << static_cast<uint8_t>(BREAK_FLAG_BIT)) | (1 << static_cast<uint8_t>(UNUSED_FLAG_BIT)));
    setFlag(INTDISABLE_FLAG_BIT, 1);

    programCounter = (readByte(IRQ_VECTOR_H) << 8) + readByte(IRQ_VECTOR_L);
    return;
};
void mos6502::BVC(uint16_t address)
{
    if (!getFlag(OVERFLOW_FLAG_BIT))
    {

        programCounter = address;
    }

    return;
};
void mos6502::BVS(uint16_t address)
{
    if (getFlag(OVERFLOW_FLAG_BIT))
    {

        programCounter = address;
    }

    return;
};
void mos6502::CLC(uint16_t address)
{
    setFlag(CARRY_FLAG_BIT, 0);

    return;
};
void mos6502::CLD(uint16_t address)
{
    setFlag(DECIMAL_FLAG_BIT, 0);

    return;
};
void mos6502::CLI(uint16_t address)
{
    setFlag(INTDISABLE_FLAG_BIT, 0);

    return;
};
void mos6502::CLV(uint16_t address)
{
    setFlag(OVERFLOW_FLAG_BIT, 0);

    return;
};
void mos6502::CMP(uint16_t address)
{

    uint8_t value = readByte(address);
    uint16_t result = accumulator - value;

    setFlag(CARRY_FLAG_BIT, accumulator >= value);
    setFlag(ZERO_FLAG_BIT, accumulator == value);
    setFlag(NEGATIVE_FLAG_BIT, (result & 0x80) != 0);

    return;
};
void mos6502::CPX(uint16_t address)
{
    uint8_t value = readByte(address);
    uint16_t result = xRegister - value;

    setFlag(CARRY_FLAG_BIT, xRegister >= value);
    setFlag(ZERO_FLAG_BIT, xRegister == value);
    setFlag(NEGATIVE_FLAG_BIT, (result & 0x80) != 0);

    return;
};
void mos6502::CPY(uint16_t address)
{
    uint8_t value = readByte(address);
    uint16_t result = yRegister - value;

    setFlag(CARRY_FLAG_BIT, yRegister >= value);
    setFlag(ZERO_FLAG_BIT, yRegister == value);
    setFlag(NEGATIVE_FLAG_BIT, (result & 0x80) != 0);

    return;
};
void mos6502::DEC(uint16_t address)
{
    uint8_t value = readByte(address);
    value = (value - 1) & 0xFF;
    setFlag(ZERO_FLAG_BIT, !value);
    setFlag(NEGATIVE_FLAG_BIT, value & 0x80);
    writeByte(address, value);
    return;
};
void mos6502::DEX(uint16_t address)
{
    xRegister--;

    setFlag(ZERO_FLAG_BIT, !xRegister);
    setFlag(NEGATIVE_FLAG_BIT, xRegister & 0x80);

    return;
};
void mos6502::DEY(uint16_t address)
{
    yRegister--;

    setFlag(ZERO_FLAG_BIT, !yRegister);
    setFlag(NEGATIVE_FLAG_BIT, yRegister & 0x80);
    return;
};
void mos6502::EOR(uint16_t address)
{

    uint8_t value = readByte(address);
    uint8_t solution = accumulator ^ value;

    setFlag(ZERO_FLAG_BIT, !solution);
    setFlag(NEGATIVE_FLAG_BIT, solution & 0x80);

    accumulator = solution;

    return;
};
void mos6502::INC(uint16_t address)
{
    uint8_t value = readByte(address);
    value++;
    setFlag(ZERO_FLAG_BIT, !value);
    setFlag(NEGATIVE_FLAG_BIT, value & 0x80);
    writeByte(address, value);

    return;
};
void mos6502::INX(uint16_t address)
{
    xRegister++;
    setFlag(ZERO_FLAG_BIT, !xRegister);
    setFlag(NEGATIVE_FLAG_BIT, xRegister & 0x80);

    return;
};
void mos6502::INY(uint16_t address)
{
    yRegister++;
    setFlag(ZERO_FLAG_BIT, !yRegister);
    setFlag(NEGATIVE_FLAG_BIT, yRegister & 0x80);

    return;
};
void mos6502::JMP(uint16_t address)
{
    programCounter = address;

    return;
};
void mos6502::JSR(uint16_t address)
{
    programCounter--;

    pushStack((programCounter >> 8) & 0xFF);
    pushStack(programCounter & 0xFF);

    programCounter = address;

    return;
};
void mos6502::LDA(uint16_t address)
{
    accumulator = readByte(address);
    setFlag(ZERO_FLAG_BIT, !accumulator);
    setFlag(NEGATIVE_FLAG_BIT, accumulator & 0x80);

    return;
};
void mos6502::LDX(uint16_t address)
{
    xRegister = readByte(address);
    setFlag(ZERO_FLAG_BIT, !xRegister);
    setFlag(NEGATIVE_FLAG_BIT, xRegister & 0x80);

    return;
};
void mos6502::LDY(uint16_t address)
{
    yRegister = readByte(address);
    setFlag(ZERO_FLAG_BIT, !yRegister);
    setFlag(NEGATIVE_FLAG_BIT, yRegister & 0x80);

    return;
};
void mos6502::LSR(uint16_t address)
{
    uint8_t value = readByte(address);

    setFlag(CARRY_FLAG_BIT, false);
    setFlag(CARRY_FLAG_BIT, value & 0x01);

    value >>= 1;

    setFlag(ZERO_FLAG_BIT, !value);
    setFlag(NEGATIVE_FLAG_BIT, false);
    writeByte(address, value);

    return;
};
void mos6502::LSR_ACC(uint16_t address)
{

    setFlag(CARRY_FLAG_BIT, accumulator & 0x01);
    accumulator >>= 1;
    setFlag(ZERO_FLAG_BIT, !accumulator);
    setFlag(NEGATIVE_FLAG_BIT, false);
};
void mos6502::NOP(uint16_t address)
{
    return;
};
void mos6502::ORA(uint16_t address)
{
    // Fetch the value from memory
    uint8_t value = readByte(address);

    // Perform the exclusive-OR operation
    accumulator |= value;

    // Update the zero flag
    setFlag(ZERO_FLAG_BIT, !accumulator);

    // Update the negative flag
    setFlag(NEGATIVE_FLAG_BIT, accumulator & 0x80);

    return;
};
void mos6502::PHA(uint16_t address)
{
    pushStack(accumulator);
    return;
};
void mos6502::PHP(uint16_t address)
{
    // Push status register onto the stack with Break flag (bit 4) and bit 5 set to 1
    pushStack(statusRegister | (1 << static_cast<uint8_t>(BREAK_FLAG_BIT)) | (1 << static_cast<uint8_t>(UNUSED_FLAG_BIT)));

    return;
};
void mos6502::PLA(uint16_t address)
{
    uint8_t value = popStack();

    accumulator = value;
    setFlag(ZERO_FLAG_BIT, !accumulator);
    setFlag(NEGATIVE_FLAG_BIT, accumulator & 0x80);

    return;
};
void mos6502::PLP(uint16_t address)
{
    uint8_t value = popStack();

    // Ignore the break flag (bit 4) and bit 5
    value &= ~(1 << static_cast<uint8_t>(BREAK_FLAG_BIT));
    value &= ~(1 << static_cast<uint8_t>(UNUSED_FLAG_BIT));
    statusRegister = value;
}
void mos6502::ROL(uint16_t address)
{
    uint16_t value = readByte(address);

    value <<= 1;
    if (getFlag(CARRY_FLAG_BIT))
        value |= 0x01;
    setFlag(CARRY_FLAG_BIT, value > 0xFF);
    value &= 0xFF;
    setFlag(ZERO_FLAG_BIT, !value);
    setFlag(NEGATIVE_FLAG_BIT, value & 0x80);

    writeByte(address, value);
};
void mos6502::ROL_ACC(uint16_t address)
{
    uint16_t value;
    value = accumulator << 1;
    if (getFlag(CARRY_FLAG_BIT))
        value |= 0x01;
    setFlag(CARRY_FLAG_BIT, value > 0xFF);
    value &= 0xFF;
    setFlag(ZERO_FLAG_BIT, !value);
    setFlag(NEGATIVE_FLAG_BIT, value & 0x80);

    accumulator = value;
};
void mos6502::ROR(uint16_t address)
{
    uint8_t value = readByte(address);
    bool oldCarry = getFlag(CARRY_FLAG_BIT);

    setFlag(CARRY_FLAG_BIT, value & 0x01);
    value = (value >> 1) | (oldCarry ? 0x80 : 0);

    setFlag(ZERO_FLAG_BIT, !value);
    setFlag(NEGATIVE_FLAG_BIT, value & 0x80);

    writeByte(address, value);

    return;
};
void mos6502::ROR_ACC(uint16_t address)
{
    bool oldCarry = getFlag(CARRY_FLAG_BIT);

    setFlag(CARRY_FLAG_BIT, accumulator & 0x01);

    accumulator = (accumulator >> 1) | (oldCarry ? 0x80 : 0x00);

    setFlag(ZERO_FLAG_BIT, !accumulator);
    setFlag(NEGATIVE_FLAG_BIT, accumulator & 0x80);

    return;
};
void mos6502::RTI(uint16_t address)
{
    // Get old status
    statusRegister = popStack();
    statusRegister | (0 << static_cast<uint8_t>(BREAK_FLAG_BIT));

    // Get return address
    uint8_t lowByte = popStack();
    uint8_t highByte = popStack();
    uint16_t returnAddress = (highByte << 8) | lowByte;
    programCounter = returnAddress;

    return;
};
void mos6502::RTS(uint16_t address)
{
    uint8_t lowByte = popStack();
    uint8_t highByte = popStack();

    uint16_t returnAddress = (highByte << 8) | lowByte;
    returnAddress++;

    programCounter = returnAddress;

    return;
};
void mos6502::SBC(uint16_t address)
{
    uint8_t value = readByte(address);
    uint16_t tmp = accumulator - value - (getFlag(CARRY_FLAG_BIT) ? 0 : 1);
    setFlag(NEGATIVE_FLAG_BIT, tmp & 0x80);
    setFlag(ZERO_FLAG_BIT, !(tmp & 0xFF));
    setFlag(OVERFLOW_FLAG_BIT, ((accumulator ^ tmp) & 0x80) && ((accumulator ^ value) & 0x80));

    if (getFlag(DECIMAL_FLAG_BIT))
    {
        if (((accumulator & 0x0F) - (getFlag(CARRY_FLAG_BIT) ? 0 : 1)) < (value & 0x0F))
            tmp -= 6;
        if (tmp > 0x99)
        {
            tmp -= 0x60;
        }
    }
    setFlag(CARRY_FLAG_BIT, tmp < 0x100);
    accumulator = (tmp & 0xFF);
    return;
};
void mos6502::SEC(uint16_t address)
{
    setFlag(CARRY_FLAG_BIT, 1);

    return;
};
void mos6502::SED(uint16_t address)
{
    setFlag(DECIMAL_FLAG_BIT, 1);

    return;
};
void mos6502::SEI(uint16_t address)
{
    setFlag(INTDISABLE_FLAG_BIT, 1);

    return;
};
void mos6502::STA(uint16_t address)
{
    writeByte(address, accumulator);

    return;
};
void mos6502::STX(uint16_t address)
{
    writeByte(address, xRegister);

    return;
};
void mos6502::STY(uint16_t address)
{
    writeByte(address, yRegister);

    return;
};
void mos6502::TAX(uint16_t address)
{
    xRegister = accumulator;
    setFlag(ZERO_FLAG_BIT, !xRegister);
    setFlag(NEGATIVE_FLAG_BIT, xRegister & 0x80);

    return;
};
void mos6502::TAY(uint16_t address)
{
    yRegister = accumulator;
    setFlag(ZERO_FLAG_BIT, !yRegister);
    setFlag(NEGATIVE_FLAG_BIT, yRegister & 0x80);

    return;
};
void mos6502::TSX(uint16_t address)
{
    xRegister = stackPointer;
    setFlag(ZERO_FLAG_BIT, !xRegister);
    setFlag(NEGATIVE_FLAG_BIT, xRegister & 0x80);

    return;
};
void mos6502::TXA(uint16_t address)
{
    accumulator = xRegister;
    setFlag(ZERO_FLAG_BIT, !accumulator);
    setFlag(NEGATIVE_FLAG_BIT, accumulator & 0x80);

    return;
};
void mos6502::TXS(uint16_t address)
{
    stackPointer = xRegister;

    return;
};
void mos6502::TYA(uint16_t address)
{
    accumulator = yRegister;
    setFlag(ZERO_FLAG_BIT, !accumulator);
    setFlag(NEGATIVE_FLAG_BIT, accumulator & 0x80);

    return;
};
void mos6502::ILLEGAL(uint16_t address) {};

#pragma endregion
#pragma region Public helper functons

// Constructor
mos6502::mos6502()
{
    programCounter = 0x0000;
    stackPointer = 0xFF;
    statusRegister = 0x36;
    accumulator = 0x00;
    xRegister = 0x00;
    yRegister = 0x00;

    // Initialize memory with nothing
    Memory.resize(65536, 0);
};

// Register helper functions

uint16_t mos6502::getPC()
{
    return programCounter;
};
void mos6502::setPC(uint16_t data)
{
    programCounter = data;
};
uint8_t mos6502::getSP()
{
    return stackPointer;
};
void mos6502::setSP(uint8_t data)
{
    stackPointer = data;
};
uint8_t mos6502::getSR()
{
    return statusRegister;
};
void mos6502::setSR(uint8_t data)
{
    statusRegister = data;
};
uint8_t mos6502::getAC()
{
    return accumulator;
};
void mos6502::setAC(uint8_t data)
{
    accumulator = data;
};
uint8_t mos6502::getXR()
{
    return xRegister;
};
void mos6502::setXR(uint8_t data)
{
    xRegister = data;
};
uint8_t mos6502::getYR()
{
    return yRegister;
};
void mos6502::setYR(uint8_t data)
{
    yRegister = data;
};

// Flag helper functions

void mos6502::setFlag(flag_bits flag, bool state)
{
    if (state)
    {
        statusRegister |= (1 << static_cast<uint8_t>(flag));
    }
    else
    {
        statusRegister &= ~(1 << static_cast<uint8_t>(flag));
    }
};
uint8_t mos6502::getFlag(flag_bits flag)
{
    switch (flag)
    {
    case flag_bits::CARRY_FLAG_BIT:
        return (statusRegister >> static_cast<uint8_t>(flag)) & 0x01;
    case flag_bits::ZERO_FLAG_BIT:
        return (statusRegister >> static_cast<uint8_t>(flag)) & 0x01;
    case flag_bits::INTDISABLE_FLAG_BIT:
        return (statusRegister >> static_cast<uint8_t>(flag)) & 0x01;
    case flag_bits::DECIMAL_FLAG_BIT:
        return (statusRegister >> static_cast<uint8_t>(flag)) & 0x01;
    case flag_bits::BREAK_FLAG_BIT:
        return (statusRegister >> static_cast<uint8_t>(flag)) & 0x01;
    case flag_bits::UNUSED_FLAG_BIT:
        return (statusRegister >> static_cast<uint8_t>(flag)) & 0x01;
    case flag_bits::OVERFLOW_FLAG_BIT:
        return (statusRegister >> static_cast<uint8_t>(flag)) & 0x01;
    case flag_bits::NEGATIVE_FLAG_BIT:
        return (statusRegister >> static_cast<uint8_t>(flag)) & 0x01;
    default:
        return 0; // Default value if an invalid flag is provided
    }
};

// Memory helper functions

uint8_t mos6502::readByte(uint16_t address)
{
    // Get data from memory
    uint8_t byte = Memory[address];
    // Return data from memory
    return byte;
};
void mos6502::writeByte(uint16_t address, uint8_t byte)
{
    // Write data to address
    Memory[address] = byte;

    return;
};

// Stack helper functions

void mos6502::pushStack(uint8_t byte)
{
    Memory[0x0100 + stackPointer] = byte;
    stackPointer = (stackPointer - 1) & 0xFF;

    return;
}
uint8_t mos6502::popStack()
{
    stackPointer = (stackPointer + 1) & 0xFF;
    return Memory[0x0100 + stackPointer];
}

// Emulation helper functions
void mos6502::loadMemory(std::vector<uint8_t> data)
{
    if (data.size() > 65536)
    {
        std::cerr << "Exceeding max load size of 65536";
        return;
    }
    Memory = data;
};
void mos6502::dumpMemory(std::string localDir)
{
    std::ofstream dumpFile(localDir); // Open the file for writing
    if (!dumpFile.is_open())
    {
        std::cerr << "Error: Unable to open file " << localDir << " for writing." << std::endl;
        return;
    }

    for (size_t i = 0; i < Memory.size(); i++)
    {
        dumpFile << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(Memory[i]);
        if (i < Memory.size() - 1) // If not the last byte, add a space
            dumpFile << " ";
    }
    dumpFile.close();
}
void mos6502::step()
{
    // Get opcode
    uint8_t opcode = Memory[programCounter++];

    // Decode opcode
    Instruction instruction = Instructions[opcode];

    // Execute opcode
    (this->*instruction.code)((this->*instruction.addr)());
}

void mos6502::reset()
{
    accumulator = 0x00;
    yRegister = 0x00;
    xRegister = 0x00;
    stackPointer = 0xFF;

    statusRegister = 0x36; // 00110110 status register

    // load PC from reset vector
    uint8_t addressLow = readByte(RESET_VECTOR_L);
    uint8_t addressHigh = readByte(RESET_VECTOR_H);
    programCounter = (addressHigh << 8) + addressLow;
    return;
};
void mos6502::IRQ()
{
    if (!getFlag(INTDISABLE_FLAG_BIT))
    {

        // Save PC and status to stack
        pushStack((programCounter >> 8) & 0xFF);
        pushStack(programCounter & 0xFF);
        pushStack(statusRegister | (1 << static_cast<uint8_t>(BREAK_FLAG_BIT)) | (1 << static_cast<uint8_t>(UNUSED_FLAG_BIT)));

        // Set intterupt flag
        setFlag(INTDISABLE_FLAG_BIT, true);

        // load PC from interrupt request vector
        uint8_t addressLow = readByte(IRQ_VECTOR_L);
        uint8_t addressHigh = readByte(IRQ_VECTOR_H);
        programCounter = (addressHigh << 8) + addressLow;
    }
};
void mos6502::NMI()
{
    pushStack((programCounter >> 8) & 0xFF);
    pushStack(programCounter & 0xFF);
    pushStack(statusRegister | (1 << static_cast<uint8_t>(BREAK_FLAG_BIT)) | (1 << static_cast<uint8_t>(UNUSED_FLAG_BIT)));

    setFlag(INTDISABLE_FLAG_BIT, true);

    uint8_t addressLow = readByte(NMI_VECTOR_L);
    uint8_t addressHigh = readByte(NMI_VECTOR_H);
    programCounter = (addressHigh << 8) + addressLow;
};

#pragma endregion