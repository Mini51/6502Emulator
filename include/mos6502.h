#ifndef mos6502_H
#define mos6502_H

#include <vector>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdlib.h>
#include <iomanip>

#define TEST_MODE_ENABLED // This is to be used when testing with 6502_65C02_functional_tests by Klaus2m5

#define NMI_VECTOR_L 0xFFFA
#define NMI_VECTOR_H 0xFFFB
#define RESET_VECTOR_L 0xFFFC
#define RESET_VECTOR_H 0xFFFD
#define IRQ_VECTOR_L 0xFFFE
#define IRQ_VECTOR_H 0xFFFF

class mos6502
{
private:
    uint16_t programCounter;
    uint8_t stackPointer;
    uint8_t statusRegister;
    uint8_t accumulator;
    uint8_t xRegister;
    uint8_t yRegister;

    std::vector<uint8_t> Memory;

    /**
     * @brief Pop a byte from the stack.
     *
     * This function pops a byte from the stack and returns it.
     *
     * @return The byte popped from the stack.
     */
    uint8_t popStack();

    /**
     * @brief Push a byte onto the stack.
     *
     * This function pushes a byte onto the stack.
     *
     * @param byte The byte to push onto the stack.
     */
    void pushStack(uint8_t byte);

#pragma region addressing + Opcodes

    /**
     * @brief Typedef for a method pointer used to execute an instruction in the MOS 6502 processor.
     *
     * This typedef represents a pointer to a member function of the `mos6502` class that takes a `uint16_t` parameter.
     * It is used to point to the method responsible for executing an instruction.
     */
    typedef void (mos6502::*CodeExec)(uint16_t);

    /**
     * @brief Typedef for a method pointer used to calculate an address for an instruction in the MOS 6502 processor.
     *
     * This typedef represents a pointer to a member function of the `mos6502` class that returns a `uint16_t` value.
     * It is used to point to the method responsible for calculating the address for an instruction.
     */
    typedef uint16_t (mos6502::*AddressExec)();

    /**
     * @brief Represents an instruction in the MOS 6502 processor.
     *
     * @param alias The mnemonic or alias of the instruction.
     * @param code A function pointer to the method responsible for executing the instruction.
     * @param addr A function pointer to the method responsible for addressing modes.
     * @param cycles The number of clock cycles required to execute the instruction.
     * @param bytes The number of bytes occupied by the instruction in memory.
     */
    struct Instruction
    {
        const char *alias;
        CodeExec code;
        AddressExec addr;
        uint8_t cycles;
        uint8_t bytes;
    };

    /**
     * @brief Addressing mode: Accumulator (ACC)
     *
     * This addressing mode uses the accumulator register as the operand.
     *
     * @param The operand address.
     */
    uint16_t addressingACC();
    /**
     * @brief Addressing mode: Immediate (IMM)
     *
     * This addressing mode uses an immediate value as the operand.
     *
     * @return The operand address.
     */
    uint16_t addressingIMM();
    /**
     * @brief Addressing mode: Absolute (ABS)
     *
     * This addressing mode provides the 16-bit address of a memory location.
     * The contents of this location are used as the operand.
     *
     * @return The operand address.
     */
    uint16_t addressingABS();
    /**
     * @brief Addressing mode: Zero-Page (ZER)
     *
     * This addressing mode provides a single-byte address for the operand,
     * with the high-byte assumed to be zero.
     *
     * @return The operand address.
     */
    uint16_t addressingZER();
    /**
     * @brief Addressing mode: Zero-Page,X (ZEX)
     *
     * This addressing mode adds the X-register to a zero-page address
     * to calculate the operand address.
     *
     * @return The operand address.
     */
    uint16_t addressingZEX();
    /**
     * @brief Addressing mode: Zero-Page,Y (ZEY)
     *
     * This addressing mode adds the Y-register to a zero-page address
     * to calculate the operand address.
     *
     * @return The operand address.
     */
    uint16_t addressingZEY();
    /**
     * @brief Addressing mode: Absolute,X (ABX)
     *
     * This addressing mode adds the X-register to an absolute address
     * to calculate the operand address.
     *
     * @return The operand address.
     */
    uint16_t addressingABX();
    /**
     * @brief Addressing mode: Absolute,Y (ABY)
     *
     * This addressing mode adds the Y-register to an absolute address
     * to calculate the operand address.
     *
     * @return The operand address.
     */
    uint16_t addressingABY();
    /**
     * @brief Addressing mode: Implied (IMP)
     *
     * This addressing mode implies the operand from the instruction itself
     * rather than from an address in memory.
     *
     * @return The operand address.
     */
    uint16_t addressingIMP();
    /**
     * @brief Addressing mode: Relative (REL)
     *
     * This addressing mode provides a relative offset to the program counter (PC),
     * which is used to calculate the operand address.
     *
     * @return The operand address.
     */
    uint16_t addressingREL();
    /**
     * @brief Addressing mode: Indexed Indirect (INX)
     *
     * This addressing mode performs indexed indirect addressing using the X-register.
     *
     * @return The operand address.
     */
    uint16_t addressingINX();
    /**
     * @brief Addressing mode: Indirect Indexed (INY)
     *
     * This addressing mode performs indirect indexed addressing using the Y-register.
     *
     * @return The operand address.
     */
    uint16_t addressingINY();
    /**
     * @brief Addressing mode: Indirect (IND)
     *
     * This addressing mode performs indirect addressing, looking up a 16-bit address
     * in memory to use as the operand.
     *
     * @return The operand address.
     */
    uint16_t addressingIND();

    // Opcodes
    /**
     * @brief Add with Carry (ADC)
     *
     * Add the contents of a memory location to the accumulator,
     * along with the carry bit.
     *
     *  @param The memory address containing the operand.
     *
     */
    void ADC(uint16_t address);
    /**
     * @brief And (AND)
     *
     * Perform a logical AND between the accumulator and
     * a memory location.
     *
     *  @param The memory address containing the operand.
     */
    void AND(uint16_t address);
    /**
     *
     * @brief Arithmetic Shift Left (ASL)
     *
     * Shift all bits in a memory location or the accumulator
     * one position to the left.
     *
     * @param address The memory address containing the operand.
     */
    void ASL(uint16_t address);
    /**
     * @brief Arithmetic Shift Left Accumulator (ASL_ACC)
     *
     * Shift all bits in the accumulator one position to the left.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void ASL_ACC(uint16_t address);
    /**
     * @brief Branch if Carry Clear (BCC)
     *
     * Branch to a relative location if the carry flag is clear.
     *
     * @param address The relative address to branch to.
     */
    void BCC(uint16_t address);
    /**
     * @brief Branch if Carry Set (BCS)
     *
     * Branch to a relative location if the carry flag is set.
     *
     * @param address The relative address to branch to.
     */
    void BCS(uint16_t address);
    /**
     * @brief Branch if Equal (BEQ)
     *
     * Branch to a relative location if the zero flag is set.
     *
     * @param address The relative address to branch to.
     */
    void BEQ(uint16_t address);
    /**
     * @brief Bit Test (BIT)
     *
     * Test the bits of a memory location with the accumulator.
     *
     * @param address The memory address to test.
     */
    void BIT(uint16_t address);
    /**
     * @brief Bracnh if Minus (BMI)
     *
     * Branch to a relative location if the negative flag is set.
     *
     * @param address The relative address to branch to.
     */
    void BMI(uint16_t address);
    /**
     * @brief Bracnh if Not Equal (BNE)
     *
     * Branch to a relative location if the zero falg is clear.
     *
     * @param address The relative address to branch to.
     */
    void BNE(uint16_t address);
    /**
     * @brief Branch on Result Plus (BPL)
     *
     * Branch to a relative location if the negative flag is clear.
     *
     * @param address The relative address to branch to.
     */
    void BPL(uint16_t address);
    /**
     * @brief Force Break (BRK)
     *
     * Initiates a software interrupt similar to a hardware interrupt (IRQ).
     * The return address pushed to the stack is PC+2, providing an extra byte of spacing
     * for a break mark (identifying a reason for the break). The status register wil be
     * pushed to the stack with the break flag set to 1. However, when retrieved during
     * RTI or by a PLP instruction, the break flag will be ignored. The interrupt disable
     * flag is not set automatically.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void BRK(uint16_t address);
    /**
     * @brief Branch on Overflow Clear (BVC)
     *
     * Branch to a relative location if the overflow flag is clear.
     *
     * @param address The relative address to branch to.
     */
    void BVC(uint16_t address);
    /**
     * @brief Branch on Overflow Set (BVS)
     *
     * Branch to a relative location if the overflow flag is set.
     *
     * @param address The relative address to branch to.
     */
    void BVS(uint16_t address);
    /**
     * @brief Clear Carry Flag (CLC)
     *
     * Clears the carry flag in the status register.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void CLC(uint16_t address);
    /**
     * @brief Clear Decimal Mode Flag (CLD)
     *
     * Clears the decimal mode flag in the status register.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void CLD(uint16_t address);
    /**
     * @brief Clear Interrupt Disable Flag (CLI)
     *
     * Clears the interrupt disable flag in the status register.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void CLI(uint16_t address);
    /**
     * @brief Clear Overflow Flag (CLV)
     *
     * Clears the overflow flag in the status register.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void CLV(uint16_t address);
    /**
     * @brief Compare Memory with Accumulator (CMP)
     *
     * Compares the value in memory with the value in the accumulator and sets the status register
     * flags based on the result.
     *
     * @param address The memory address containing the operand.
     */
    void CMP(uint16_t address);
    /**
     * @brief Compare Memory and Index X (CPX)
     *
     * Compares the value in memory with the value in the X register and sets the status register
     * flags based on the result.
     *
     * @param address The memory address containing the operand.
     */
    void CPX(uint16_t address);
    /**
     * @brief Compare Memory and Index Y (CPY)
     *
     * Compares the value in memory with the value in the Y register and sets the status register
     * flags based on the result.
     *
     * @param address The memory address containing the operand.
     */
    void CPY(uint16_t address);
    /**
     * @brief Decrement Memory (DEC)
     *
     * Decrements the value stored at the specified memory address by one.
     *
     * @param address The memory address containing the operand.
     */
    void DEC(uint16_t address);
    /**
     * @brief Decrement X Register (DEX)
     *
     * Decrements the X register by one.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void DEX(uint16_t address);
    /**
     * @brief Decrement Y Register (DEY)
     *
     * Decrements the Y register by one.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void DEY(uint16_t address);
    /**
     * @brief Exclusive OR (EOR)
     *
     * Performs an exclusive OR operation between the accumulator and the value stored at the specified memory address.
     *
     * @param addressThe memory address containing the operand.
     */
    void EOR(uint16_t address);
    /**
     * @brief Increment Memory (INC)
     *
     * Increments the value stored at the specified memory address by one.
     *
     * @param address The memory address containing the operand.
     */
    void INC(uint16_t address);
    /**
     * @brief Increment X Register (INX)
     *
     * Increments the X register by one.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void INX(uint16_t address);
    /**
     * @brief Increment Y Register (INY)
     *
     * Increments the Y register by one.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void INY(uint16_t address);
    /**
     * @brief Jump to New Location (JMP)
     *
     * Sets the program counter to the specified address.
     *
     * @param address The address to jump to.
     */
    void JMP(uint16_t address);
    /**
     * @brief Jump to Subroutine (JSR)
     *
     * Pushes the address of the next instruction onto the stack and then sets the program counter to thespecified address.
     *
     * @param address The address to jump to.
     */
    void JSR(uint16_t address);
    /**
     * @brief Load Accumulator (LDA)
     *
     * Loads the accumulator with the value stored at the specified memory address.
     *
     * @param address The memory address containing the operand.
     */
    void LDA(uint16_t address);
    /**
     * @brief Load X Register (LDX)
     *
     * Loads the X register with the value stored at the specified memory address.
     *
     * @param address The memory address containing the operand.
     */
    void LDX(uint16_t address);
    /**
     * @brief Load Y Register (LDY)
     *
     * Loads the Y register with the value stored at the specified memory address.
     *
     * @param address The memory address containing the operand.
     */
    void LDY(uint16_t address);
    /**
     * @brief Logical Shift Right (LSR)
     *
     * Shifts the bits of the value stored at the specified memory address one bit to the right.
     *
     * @param address The memory address containing the operand.
     */
    void LSR(uint16_t address);
    /**
     * @brief Logical Shift Right (LSR) Accumulator
     *
     * Shifts the bits of the accumulator one bit to the right.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void LSR_ACC(uint16_t address);
    /**
     * @brief No Operation (NOP)
     *
     * Does nothing. A placeholder instruction.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void NOP(uint16_t address);
    /**
     * @brief Logical OR (ORA)
     *
     * Performs a logical OR operation between the accumulator and the value stored at the specified memory address.
     *
     * @param address The memory address containing the operand.
     */
    void ORA(uint16_t address);
    /**
     * @brief Push Accumulator on Stack (PHA)
     *
     * Pushes the value of the accumulator onto the stack.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void PHA(uint16_t address);
    /**
     * @brief Push Processor Status (PHP)
     *
     * Pushes the current status register onto the stack.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void PHP(uint16_t address);
    /**
     * @brief Pull Accumulator (PLA)
     *
     * Pulls the top byte from the stack and stores it into the accumulator register,
     * effectively restoring the value of the accumulator prior to an interrupt.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void PLA(uint16_t address);
    /**
     * @brief Pull Processor Status (PLP)
     *
     * Pulls the processor status (flags) from the stack into the status register,
     * effectively restoring the state of the flags prior to an interrupt.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void PLP(uint16_t address);
    /**
     * @brief Rotate Left (ROL)
     *
     * Rotate the bits of a memory location or accumulator one position to the left,
     * with the carry bit being shifted into bit 0 and the original bit 7 shifted into the carry.
     *
     * @param address The memory address containing the operand.
     */
    void ROL(uint16_t address);
    /**
     * @brief Rotate Left Accumulator (ROL_ACC)
     *
     * Rotate the bits of the accumulator (A register) one position to the left,
     * with the carry bit being shifted into bit 7 and the original bit 0 shifted into the carry.
     *
     * @param address The memory address containing the operand.
     */
    void ROL_ACC(uint16_t address);
    /**
     * @brief Rotate Right (ROR)
     *
     * Rotate the bits of a memory location one position to the right,
     * with the carry bit being shifted into bit 0 and the original bit 7 shifted into the carry.
     *
     * @param address The memory address containing the operand.
     */
    void ROR(uint16_t address);
    /**
     * @brief Rotate Right Accumulator (ROR_ACC)
     *
     * Rotate the bits of the accumulator one position to the right,
     * with the carry bit being shifted into bit 7 and the original bit 0 shifted into the carry.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void ROR_ACC(uint16_t address);
    /**
     * @brief Return from Interrupt (RTI)
     *
     * Restores the processor state from the stack after an interrupt service routine,
     * including the program counter and status register, and resumes execution.
     *
     * @param address The return address from the interrupt service routine.
     */
    void RTI(uint16_t address);
    /**
     * @brief Return from Subroutine (RTS)
     *
     * Restores the program counter from the stack after a subroutine call,
     * allowing execution to resume at the instruction following the one that called the subroutine.
     *
     * @param address The return address from the subroutine call.
     */
    void RTS(uint16_t address);
    /**
     * @brief Subtract With Carry (SBC)
     *
     * Subtracts the value stored at the memory address from the accumulator,
     * along with the carry bit if it is set, and stores the result in the accumulator.
     *
     * @param address The memory address containing the operand.
     */
    void SBC(uint16_t address);
    /**
     * @brief Set Carry Flag (SEC)
     *
     * Sets the Carry Flag to 1.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void SEC(uint16_t address);
    /**
     * @brief Set Decimal Flag (SED)
     *
     * Sets the Decimal Flag to 1.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void SED(uint16_t address);
    /**
     * @brief Set Interrupt Disable Flag (SEI)
     *
     * Sets the Interrupt Disable Flag to 1.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void SEI(uint16_t address);
    /**
     * @brief Store Accumulator to Memory (STA)
     *
     * Stores value in the Accumulator into memory.
     *
     * @param address The location to store to in memory.
     */
    void STA(uint16_t address);
    /**
     * @brief Store Index X to Memory (STX)
     *
     * Stores value in the X register into memory.
     *
     * @param address The location to store to in memory.
     */
    void STX(uint16_t address);
    /**
     * @brief Store Index Y to Memory (STY)
     *
     * Stores value in the Y register into memory.
     *
     * @param address The location to store to in memory.
     */
    void STY(uint16_t address);
    /**
     * @brief Transfer Accumulator to Index X (TAX)
     *
     * Transfers value in the Accumulator into the X register.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void TAX(uint16_t address);
    /**
     * @brief Transfer Accumulator to Index Y (TAY)
     *
     * Transfers value in Accumulator to Y regsister.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void TAY(uint16_t address);
    /**
     * @brief Transfer Stack Pointer to Index X (TSX)
     *
     * Transfers value in the Stack Pointer into the X register.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void TSX(uint16_t address);
    /**
     * @brief Transfer Index X to Accumulator
     *
     * Transfers value in the X register into the Accumulator
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void TXA(uint16_t address);
    /**
     * @brief Transfer Index X to Stack Register (TXS)
     *
     * Transfers value in the X register into the Status Register.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void TXS(uint16_t address);
    /**
     * @brief Transfer Index Y to Accumulator (TYA)
     *
     * Transfers value in the Y register into the Accumulator.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void TYA(uint16_t address);
    /**
     * @brief ILLEGAL opcode
     *
     * Handler for illigal opcodes.
     *
     * @param address Unused parameter for consistency with other instruction signatures.
     */
    void ILLEGAL(uint16_t address);

    /**
     * @brief List of all opcodes supported by the MOS 6502 processor.
     *
     */
    Instruction Instructions[256] = {
        {"BRK", &mos6502::BRK, &mos6502::addressingIMP, 7, 1},
        {"ORA", &mos6502::ORA, &mos6502::addressingINX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingZER, 3, 1},
        {"ORA", &mos6502::ORA, &mos6502::addressingZER, 3, 2},
        {"ASL", &mos6502::ASL, &mos6502::addressingZER, 5, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"PHP", &mos6502::PHP, &mos6502::addressingIMP, 3, 1},
        {"ORA", &mos6502::ORA, &mos6502::addressingIMM, 2, 2},
        {"ASL_ACC", &mos6502::ASL_ACC, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingABS, 4, 1},
        {"ORA", &mos6502::ORA, &mos6502::addressingABS, 4, 2},
        {"ASL", &mos6502::ASL, &mos6502::addressingABS, 6, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BPL", &mos6502::BPL, &mos6502::addressingREL, 2, 2},
        {"ORA", &mos6502::ORA, &mos6502::addressingINY, 5, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingZEX, 4, 1},
        {"ORA", &mos6502::ORA, &mos6502::addressingZEX, 4, 2},
        {"ASL", &mos6502::ASL, &mos6502::addressingZEX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CLC", &mos6502::CLC, &mos6502::addressingIMP, 2, 1},
        {"ORA", &mos6502::ORA, &mos6502::addressingABY, 4, 3},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingABX, 4, 1},
        {"ORA", &mos6502::ORA, &mos6502::addressingABX, 4, 3},
        {"ASL", &mos6502::ASL, &mos6502::addressingABX, 7, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"JSR", &mos6502::JSR, &mos6502::addressingABS, 6, 3},
        {"AND", &mos6502::AND, &mos6502::addressingINX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BIT", &mos6502::BIT, &mos6502::addressingZER, 3, 2},
        {"AND", &mos6502::AND, &mos6502::addressingZER, 3, 2},
        {"ROL", &mos6502::ROL, &mos6502::addressingZER, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"PLP", &mos6502::PLP, &mos6502::addressingIMP, 4, 1},
        {"AND", &mos6502::AND, &mos6502::addressingIMM, 2, 2},
        {"ROL_ACC", &mos6502::ROL_ACC, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BIT", &mos6502::BIT, &mos6502::addressingABS, 4, 3},
        {"AND", &mos6502::AND, &mos6502::addressingABS, 4, 3},
        {"ROL", &mos6502::ROL, &mos6502::addressingABS, 6, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BMI", &mos6502::BMI, &mos6502::addressingREL, 2, 2},
        {"AND", &mos6502::AND, &mos6502::addressingINY, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingZEX, 4, 1},
        {"AND", &mos6502::AND, &mos6502::addressingZEX, 4, 2},
        {"ROL", &mos6502::ROL, &mos6502::addressingZEX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"SEC", &mos6502::SEC, &mos6502::addressingIMP, 2, 1},
        {"AND", &mos6502::AND, &mos6502::addressingABY, 4, 3},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingABX, 4, 1},
        {"AND", &mos6502::AND, &mos6502::addressingABX, 4, 3},
        {"ROL", &mos6502::ROL, &mos6502::addressingABX, 7, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"RTI", &mos6502::RTI, &mos6502::addressingIMP, 6, 1},
        {"EOR", &mos6502::EOR, &mos6502::addressingINX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingZER, 3, 1},
        {"EOR", &mos6502::EOR, &mos6502::addressingZER, 3, 2},
        {"LSR", &mos6502::LSR, &mos6502::addressingZER, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"PHA", &mos6502::PHA, &mos6502::addressingIMP, 3, 1},
        {"EOR", &mos6502::EOR, &mos6502::addressingIMM, 2, 2},
        {"LSR_ACC", &mos6502::LSR_ACC, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"JMP", &mos6502::JMP, &mos6502::addressingABS, 3, 3},
        {"EOR", &mos6502::EOR, &mos6502::addressingABS, 4, 3},
        {"LSR", &mos6502::LSR, &mos6502::addressingABS, 6, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BVC", &mos6502::BVC, &mos6502::addressingREL, 2, 2},
        {"EOR", &mos6502::EOR, &mos6502::addressingINY, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingZEX, 4, 1},
        {"EOR", &mos6502::EOR, &mos6502::addressingZEX, 4, 2},
        {"LSR", &mos6502::LSR, &mos6502::addressingZEX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CLI", &mos6502::CLI, &mos6502::addressingIMP, 2, 1},
        {"EOR", &mos6502::EOR, &mos6502::addressingABY, 4, 3},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingABX, 4, 1},
        {"EOR", &mos6502::EOR, &mos6502::addressingABX, 4, 1},
        {"LSR", &mos6502::LSR, &mos6502::addressingABX, 7, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"RTS", &mos6502::RTS, &mos6502::addressingIMP, 6, 1},
        {"ADC", &mos6502::ADC, &mos6502::addressingINX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingZER, 3, 1},
        {"ADC", &mos6502::ADC, &mos6502::addressingZER, 3, 2},
        {"ROR", &mos6502::ROR, &mos6502::addressingZER, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"PLA", &mos6502::PLA, &mos6502::addressingIMP, 4, 1},
        {"ADC", &mos6502::ADC, &mos6502::addressingIMM, 2, 2},
        {"ROR_ACC", &mos6502::ROR_ACC, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"JMP", &mos6502::JMP, &mos6502::addressingIND, 6, 3},
        {"ADC", &mos6502::ADC, &mos6502::addressingABS, 4, 3},
        {"ROR", &mos6502::ROR, &mos6502::addressingABS, 6, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BVS", &mos6502::BVS, &mos6502::addressingREL, 2, 2},
        {"ADC", &mos6502::ADC, &mos6502::addressingINY, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingZEX, 4, 1},
        {"ADC", &mos6502::ADC, &mos6502::addressingZEX, 4, 2},
        {"ROR", &mos6502::ROR, &mos6502::addressingZEX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"SEI", &mos6502::SEI, &mos6502::addressingIMP, 2, 1},
        {"ADC", &mos6502::ADC, &mos6502::addressingABY, 4, 3},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingABX, 4, 1},
        {"ROC", &mos6502::ADC, &mos6502::addressingABX, 4, 3},
        {"ROR", &mos6502::ROR, &mos6502::addressingABX, 7, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMM, 2, 1},
        {"STA", &mos6502::STA, &mos6502::addressingINX, 6, 2},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMM, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"STY", &mos6502::STY, &mos6502::addressingZER, 3, 2},
        {"STA", &mos6502::STA, &mos6502::addressingZER, 3, 2},
        {"STX", &mos6502::STX, &mos6502::addressingZER, 3, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"DEY", &mos6502::DEY, &mos6502::addressingIMP, 2, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMM, 2, 1},
        {"TXA", &mos6502::TXA, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"STY", &mos6502::STY, &mos6502::addressingABS, 4, 3},
        {"STA", &mos6502::STA, &mos6502::addressingABS, 4, 3},
        {"STX", &mos6502::STX, &mos6502::addressingABS, 4, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BCC", &mos6502::BCC, &mos6502::addressingREL, 2, 2},
        {"STA", &mos6502::STA, &mos6502::addressingINY, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"STY", &mos6502::STY, &mos6502::addressingZEX, 4, 2},
        {"STA", &mos6502::STA, &mos6502::addressingZEX, 4, 2},
        {"STX", &mos6502::STX, &mos6502::addressingZEY, 4, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"TYA", &mos6502::TYA, &mos6502::addressingIMP, 2, 1},
        {"STA", &mos6502::STA, &mos6502::addressingABY, 5, 3},
        {"TXS", &mos6502::TXS, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"STA", &mos6502::STA, &mos6502::addressingABX, 5, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"LDY", &mos6502::LDY, &mos6502::addressingIMM, 2, 2},
        {"LDA", &mos6502::LDA, &mos6502::addressingINX, 6, 2},
        {"LDX", &mos6502::LDX, &mos6502::addressingIMM, 2, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"LDY", &mos6502::LDY, &mos6502::addressingZER, 3, 2},
        {"LDA", &mos6502::LDA, &mos6502::addressingZER, 3, 2},
        {"LDX", &mos6502::LDX, &mos6502::addressingZER, 3, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"TAY", &mos6502::TAY, &mos6502::addressingIMP, 2, 1},
        {"LDA", &mos6502::LDA, &mos6502::addressingIMM, 2, 2},
        {"TAX", &mos6502::TAX, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"LDY", &mos6502::LDY, &mos6502::addressingABS, 4, 3},
        {"LDA", &mos6502::LDA, &mos6502::addressingABS, 4, 3},
        {"LDX", &mos6502::LDX, &mos6502::addressingABS, 4, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BCS", &mos6502::BCS, &mos6502::addressingREL, 2, 2},
        {"LDA", &mos6502::LDA, &mos6502::addressingINY, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"LDY", &mos6502::LDY, &mos6502::addressingZEX, 4, 2},
        {"LDA", &mos6502::LDA, &mos6502::addressingZEX, 4, 2},
        {"LDX", &mos6502::LDX, &mos6502::addressingZEY, 4, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CLV", &mos6502::CLV, &mos6502::addressingIMP, 2, 1},
        {"LDA", &mos6502::LDA, &mos6502::addressingABY, 4, 3},
        {"TSX", &mos6502::TSX, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"LDY", &mos6502::LDY, &mos6502::addressingABX, 4, 3},
        {"LDA", &mos6502::LDA, &mos6502::addressingABX, 4, 3},
        {"LDX", &mos6502::LDX, &mos6502::addressingABY, 4, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CPY", &mos6502::CPY, &mos6502::addressingIMM, 2, 2},
        {"CMP", &mos6502::CMP, &mos6502::addressingINX, 6, 2},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMM, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CPY", &mos6502::CPY, &mos6502::addressingZER, 3, 2},
        {"CMP", &mos6502::CMP, &mos6502::addressingZER, 3, 2},
        {"DEC", &mos6502::DEC, &mos6502::addressingZER, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"INY", &mos6502::INY, &mos6502::addressingIMP, 2, 1},
        {"CMP", &mos6502::CMP, &mos6502::addressingIMM, 2, 2},
        {"DEX", &mos6502::DEX, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CPY", &mos6502::CPY, &mos6502::addressingABS, 4, 3},
        {"CMP", &mos6502::CMP, &mos6502::addressingABS, 4, 3},
        {"DEC", &mos6502::DEC, &mos6502::addressingABS, 6, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BNE", &mos6502::BNE, &mos6502::addressingREL, 2, 2},
        {"CMP", &mos6502::CMP, &mos6502::addressingINY, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingZEX, 4, 1},
        {"CMP", &mos6502::CMP, &mos6502::addressingZEX, 4, 2},
        {"DEC", &mos6502::DEC, &mos6502::addressingZEX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CLD", &mos6502::CLD, &mos6502::addressingIMP, 2, 1},
        {"CMP", &mos6502::CMP, &mos6502::addressingABY, 4, 3},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingABX, 4, 1},
        {"CMP", &mos6502::CMP, &mos6502::addressingABX, 4, 3},
        {"DEC", &mos6502::DEC, &mos6502::addressingABX, 7, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CPX", &mos6502::CPX, &mos6502::addressingIMM, 2, 2},
        {"SBC", &mos6502::SBC, &mos6502::addressingINX, 6, 2},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMM, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CPX", &mos6502::CPX, &mos6502::addressingZER, 3, 2},
        {"SBC", &mos6502::SBC, &mos6502::addressingZER, 3, 2},
        {"INC", &mos6502::INC, &mos6502::addressingZER, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"INX", &mos6502::INX, &mos6502::addressingIMP, 2, 1},
        {"SBC", &mos6502::SBC, &mos6502::addressingIMM, 2, 2},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"CPX", &mos6502::CPX, &mos6502::addressingABS, 4, 3},
        {"SBC", &mos6502::SBC, &mos6502::addressingABS, 4, 3},
        {"INC", &mos6502::INC, &mos6502::addressingABS, 6, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"BEQ", &mos6502::BEQ, &mos6502::addressingREL, 2, 2},
        {"SBC", &mos6502::SBC, &mos6502::addressingINY, 5, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingZEX, 4, 1},
        {"SBC", &mos6502::SBC, &mos6502::addressingZEX, 4, 2},
        {"INC", &mos6502::INC, &mos6502::addressingZEX, 6, 2},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"SED", &mos6502::SED, &mos6502::addressingIMP, 2, 1},
        {"SBC", &mos6502::SBC, &mos6502::addressingABY, 4, 3},
        {"NOP", &mos6502::NOP, &mos6502::addressingIMP, 2, 1},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
        {"NOP", &mos6502::NOP, &mos6502::addressingABX, 4, 1},
        {"SBC", &mos6502::SBC, &mos6502::addressingABX, 4, 3},
        {"INC", &mos6502::INC, &mos6502::addressingABX, 7, 3},
        {"ILG", &mos6502::ILLEGAL, &mos6502::addressingIMP, 0, 1},
    };

#pragma endregion

public:
    mos6502();

    /***
     * @brief Get the value of the program counter (PC).
     *
     * @return The value of the program counter.
     */
    uint16_t getPC();

    /***
     * @brief Set the value of the program counter (PC).
     *
     * @param data The value to set the program counter to.
     */
    void setPC(uint16_t data);

    /***
     * @brief Get the value of the stack pointer (SP).
     *
     * @return The value of the stack pointer.
     */
    uint8_t getSP();

    /***
     * @brief Set the value of the stack pointer (SP).
     *
     * @param data The value to set the stack pointer to.
     */
    void setSP(uint8_t data);

    /***
     * @brief Get the value of the status register (SR).
     *
     * @return The value of the status register.
     */
    uint8_t getSR();

    /***
     * @brief Set the value of the status register (SR).
     *
     * @param data The value to set the status register to.
     */
    void setSR(uint8_t data);

    /***
     * @brief Get the value of the accumulator (AC).
     *
     * @return The value of the accumulator.
     */
    uint8_t getAC();

    /***
     * @brief Set the value of the accumulator (AC).
     *
     * @param data The value to set the accumulator to.
     */
    void setAC(uint8_t data);

    /***
     * @brief Get the value of the X register (XR).
     *
     * @return The value of the X register.
     */
    uint8_t getXR();

    /***
     * @brief Set the value of the X register (XR).
     *
     * @param data The value to set the X register to.
     */
    void setXR(uint8_t data);

    /***
     * @brief Get the value of the Y register (YR).
     *
     * @return The value of the Y register.
     */
    uint8_t getYR();

    /***
     * @brief Set the value of the Y register (YR).
     *
     * @param data The value to set the Y register to.
     */
    void setYR(uint8_t data);

    /**
     * @brief Enumeration of flag bits in the status register.
     */
    enum flag_bits : const uint8_t
    {
        CARRY_FLAG_BIT = 0,      ///< Carry flag bit index
        ZERO_FLAG_BIT = 1,       ///< Zero flag bit index
        INTDISABLE_FLAG_BIT = 2, ///< Interrupt disable flag bit index
        DECIMAL_FLAG_BIT = 3,    ///< Decimal mode flag bit index
        BREAK_FLAG_BIT = 4,      ///< Break command flag bit index
        UNUSED_FLAG_BIT = 5,     ///< Unused flag bit index
        OVERFLOW_FLAG_BIT = 6,   ///< Overflow flag bit index
        NEGATIVE_FLAG_BIT = 7,   ///< Negative flag bit index
    };

    /**
     * @brief Set the specified flag to the given state.
     *
     * @param flag The flag to set.
     * @param state The state to set the flag to (true for set, false for clear).
     */
    void setFlag(flag_bits flag, bool state);

    /**
     * @brief Get the value of the specified flag.
     *
     * @param flag The flag to get.
     * @return The value of the flag (1 if set, 0 if clear).
     */
    uint8_t getFlag(flag_bits flag);

    /**
     * @brief Read a byte from the specified memory address.
     *
     * @param address The memory address to read from.
     * @return The byte read from the memory address.
     */
    uint8_t readByte(uint16_t address);

    /**
     * @brief Write a byte to the specified memory address.
     *
     * @param address The memory address to write to.
     * @param data The byte to write to the memory address.
     */
    void writeByte(uint16_t address, uint8_t data);

    /**
     * @brief Load memory with the provided data.
     *
     * @param data The data to load into memory.
     */
    void loadMemory(std::vector<uint8_t> data);

    /**
     * @brief Dump memory to a file in the specified directory.
     *
     * @param localDir The directory to dump memory contents to.
     */
    void dumpMemory(std::string localDir);

    /**
     * @brief Execute one instruction.
     */
    void step();

    /**
     * @brief Reset the CPU.
     */
    void reset();

    /**
     * @brief Trigger an Interrupt Request (IRQ).
     */
    void IRQ();

    /**
     * @brief Trigger a Non-Maskable Interrupt (NMI).
     */
    void NMI();
};

#endif