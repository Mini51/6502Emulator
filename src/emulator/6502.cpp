#include "6502.h"
#include <unistd.h>



#define NMIVector 0xFFFA
#define RESETVector 0xFFFC
#define IRQVector 0xFFFE

cmos6502::cmos6502(){ 
    AC = 0x00; 
    XR = 0x00;
    YR = 0x00;
    SP = 0xFF;
    SR = 0x36;
    
    InstructionCount = 0;
    CycleCount = 0;
    Running = false;

};


// Addressing Modes
uint16_t cmos6502::Add_ACC(){
    return 0;
}
uint16_t cmos6502::Add_IMM(){
    PC++;

    return PC;
}
uint16_t cmos6502::Add_ABS(){
    uint8_t low = ReadByte(PC++); 
    uint8_t high = ReadByte(PC++);

    return (high << 8) | low;
}
uint16_t cmos6502::Add_ZER(){
    return ReadByte(PC++);
}
uint16_t cmos6502::Add_ZEX(){
    return (ReadByte(PC++) + XR) & 0xFF;
}
uint16_t cmos6502::Add_ZEY(){
    return (ReadByte(PC++) + YR) & 0xFF;
}
uint16_t cmos6502::Add_ABX(){
    uint8_t low = ReadByte(PC++); 
    uint8_t high = ReadByte(PC++);

    return ((high << 8) | low) + XR;
}
uint16_t cmos6502::Add_ABY(){
    uint8_t low = ReadByte(PC++); 
    uint8_t high = ReadByte(PC++);

    return ((high << 8) | low) + YR;

}
uint16_t cmos6502::Add_IMP(){
    return PC;
}
uint16_t cmos6502::Add_REL(){
    uint16_t offset; 
    uint16_t addr;

	offset = (uint16_t)ReadByte(PC++);
	if (offset & 0x80) offset |= 0xFF00;
	addr = PC + (int16_t)offset;
	return addr;
}
uint16_t cmos6502::Add_INX(){
    uint16_t zeroL;
    uint16_t zeroH;
    uint16_t addr;

    zeroL = ReadByte(PC++);
    zeroH = (zeroL + 1) & 0xFF;
    addr = ReadByte(zeroL + XR) + (ReadByte(zeroH + XR) << 8);

    return addr;
}
uint16_t cmos6502::Add_INY(){
	uint16_t zeroL;
	uint16_t zeroH;
	uint16_t addr;

	zeroL = ReadByte(PC++);
	zeroH = (zeroL + 1) & 0xFF;
	addr = ReadByte(zeroL) + (ReadByte(zeroH) << 8) + YR;

	return addr;
}
uint16_t cmos6502::Add_ABI(){
    uint16_t addrL;
	uint16_t addrH;
	uint16_t effL;
	uint16_t effH;
	uint16_t abs;
	uint16_t addr;

	addrL = ReadByte(PC++);
	addrH = ReadByte(PC++);

	abs = (addrH << 8) | addrL;

	effL = ReadByte(abs);

	effH = ReadByte((abs & 0xFF00) + ((abs + 1) & 0x00FF) );

	addr = effL + 0x100 * effH;

	return addr;

}

// Opcodes
void cmos6502::Op_ADC(uint16_t src){
    uint8_t data = ReadByte(src);
    unsigned tmp = AC + data + (CheckFlag(CARRY));
    SetFlag(ZERO, !(tmp & 0xFF));
    SetFlag(NEGATIVE, tmp & 0x80);
    SetFlag(OVERFLOW, ((AC ^ tmp) & (AC ^ data)) & 0x80);

    if(CheckFlag(DECIMAL)){ 
        if(((AC & 0xF) + (data & 0xF) + (CheckFlag(CARRY))) > 9){
            tmp += 6;
        }
        if(tmp > 0x99){
            tmp += 0x60;
        }
    }
    SetFlag(CARRY, tmp > 0xFF);
    AC = tmp & 0xFF;

    return;
};
void cmos6502::Op_AND(uint16_t src){
    //Get data from memory
    uint8_t data = ReadByte(src);
    //Set the accumulator to the accumulator AND the data
    AC &= data;
    //Set the zero flag if the accumulator is zero
    SetFlag(ZERO, !AC);
    //Set the negative flag if the accumulator is negative
    SetFlag(NEGATIVE, AC & 0x80);

    return;
};
void cmos6502::Op_ASL(uint16_t src){
    //Get data from memory
    uint8_t data = ReadByte(src);
    //Set the carry flag if the data is greater than or equal to 0x80
    SetFlag(CARRY, data & 0x80);
    //Shift the data left
    data <<= 1;
    //Write the data back to memory
    WriteByte(src, data);
    //Set the zero flag if the data is zero
    SetFlag(ZERO, !data);
    //Set the negative flag if the data is negative
    SetFlag(NEGATIVE, data & 0x80);

    return;
};
void cmos6502::Op_ASL_ACC(uint16_t src){
    //Set the carry flag if the accumulator is greater than or equal to 0x80
    SetFlag(CARRY, AC & 0x80);
    //Shift the accumulator left
    AC <<= 1;
    //Set the zero flag if the accumulator is zero
    SetFlag(ZERO, !AC);
    //Set the negative flag if the accumulator is negative
    SetFlag(NEGATIVE, AC & 0x80);

    return;
};
void cmos6502::Op_BCC(uint16_t src){
    if(!CheckFlag(CARRY)){
        PC = src;
    }
};
void cmos6502::Op_BCS(uint16_t src){
    if(CheckFlag(CARRY)){
        PC = src;
    }
};
void cmos6502::Op_BEQ(uint16_t src){
    if(CheckFlag(ZERO)){
        PC = src;
    }
};
void cmos6502::Op_BIT(uint16_t src){    
    uint8_t data = ReadByte(src);
    SetFlag(ZERO, !(AC & data));
    SetFlag(NEGATIVE, data & 0x80);
    SetFlag(OVERFLOW, data & 0x40);

    return;

};
void cmos6502::Op_BMI(uint16_t src){
    if(CheckFlag(NEGATIVE)){
        PC = src;
    }
};
void cmos6502::Op_BNE(uint16_t src){
    if(!CheckFlag(ZERO)){
        PC = src;
    }
};
void cmos6502::Op_BPL(uint16_t src){
    if(!CheckFlag(NEGATIVE)){
        PC = src;
    }    
};
void cmos6502::Op_BRK(uint16_t src){
    PC++;
    StackPush(PC >> 8);
    StackPush(PC & 0xFF);
    StackPush(SR);
    SetFlag(INTERRUPT, true);
    PC = ReadByte(0xFFFE) | (ReadByte(0xFFFF) << 8);

    return;
};
void cmos6502::Op_BVC(uint16_t src){
    if(!CheckFlag(OVERFLOW)){
        PC = src;
    }
};
void cmos6502::Op_BVS(uint16_t src){
    if(CheckFlag(OVERFLOW)){
        PC = src;
    }
};
void cmos6502::Op_CLC(uint16_t src){
    SetFlag(CARRY, false);

    return;
};
void cmos6502::Op_CLD(uint16_t src){
    SetFlag(DECIMAL, false);

    return;
};
void cmos6502::Op_CLI(uint16_t src){
    SetFlag(INTERRUPT, false);

    return;
};
void cmos6502::Op_CLV(uint16_t src){
    SetFlag(OVERFLOW, false);

    return;
};
void cmos6502::Op_CMP(uint16_t src){
    //Get data from memory
    unsigned temp = AC - ReadByte(src);
    //Set the zero flag if the data is zero
    SetFlag(ZERO, !(temp & 0xFF));
    //Set the negative flag if the data is negative
    SetFlag(NEGATIVE, temp & 0x80);
    //Set the carry flag if the data is greater than or equal to the accumulator
    SetFlag(CARRY, AC >= ReadByte(src));

    return;
};
void cmos6502::Op_CPX(uint16_t src){
    //Get data from memory
    unsigned temp = XR - ReadByte(src);
    //Set the zero flag if the data is zero
    SetFlag(ZERO, !(temp & 0xFF));
    //Set the negative flag if the data is negative
    SetFlag(NEGATIVE, temp & 0x80);
    //Set the carry flag if the data is greater than or equal to the X register
    SetFlag(CARRY, XR >= ReadByte(src));


    return;
};
void cmos6502::Op_CPY(uint16_t src){
    //Get data from memory
    unsigned temp = YR - ReadByte(src);
    //Set the zero flag if the data is zero
    SetFlag(ZERO, !(temp & 0xFF));
    //Set the negative flag if the data is negative
    SetFlag(NEGATIVE, temp & 0x80);
    //Set the carry flag if the data is greater than or equal to the Y register
    SetFlag(CARRY, YR >= ReadByte(src));

    return;
};
void cmos6502::Op_DEC(uint16_t src){
    //Get data from memory 
    uint8_t data = ReadByte(src); 
    //Decrement the data
    data--;
    //Write the data back to memory
    WriteByte(src, data);
    //Set the zero flag if the data is zero
    SetFlag(ZERO, !data);
    //Set the negative flag if the data is negative
    SetFlag(NEGATIVE, data & 0x80);
};
void cmos6502::Op_DEX(uint16_t src){
    XR--; 
    SetFlag(ZERO, !XR);
    SetFlag(NEGATIVE, XR & 0x80);

};
void cmos6502::Op_DEY(uint16_t src){
    YR--;
    SetFlag(ZERO, YR == 0x00);
    SetFlag(NEGATIVE, YR & 0x80);

    return;
};
void cmos6502::Op_EOR(uint16_t src){
    //Get data from memory
    uint8_t data = ReadByte(src);
    //Set the accumulator to the accumulator XOR the data
    AC ^= data;
    //Set the zero flag if the accumulator is zero
    SetFlag(ZERO, !AC);
    //Set the negative flag if the accumulator is negative
    SetFlag(NEGATIVE, AC & 0x80);

    return;
};
void cmos6502::Op_INC(uint16_t src){
    //Get data from memory 
    uint8_t data = ReadByte(src); 
    //Increment the data
    data++;
    //Write the data back to memory
    WriteByte(src, data);
    //Set the zero flag if the data is zero
    SetFlag(ZERO, !data);
    //Set the negative flag if the data is negative
    SetFlag(NEGATIVE, data & 0x80);
};
void cmos6502::Op_INX(uint16_t src){
    XR++;
    SetFlag(ZERO, XR == 0x00);
    SetFlag(NEGATIVE, XR & 0x80);

    return;
};
void cmos6502::Op_INY(uint16_t src){
    YR++;
    SetFlag(ZERO, YR == 0x00);
    SetFlag(NEGATIVE, YR & 0x80);

    return;
};  
void cmos6502::Op_JMP(uint16_t src){
    PC = src;

    return;
};
void cmos6502::Op_JSR(uint16_t src){
    PC--;
    StackPush(PC >> 8);
    StackPush(PC & 0xFF);
    PC = src;

    return;
};
void cmos6502::Op_LDA(uint16_t src){
    //Get data from memory 
    uint8_t data = ReadByte(src); 
    //Set the accumulator to the data
    AC = data;

    return;
};
void cmos6502::Op_LDX(uint16_t src){
    //Get data from memory 
    uint8_t data = ReadByte(src);
    //Set the X register to the data
    XR = data;

    return;
};
void cmos6502::Op_LDY(uint16_t src){
    //Get data from memory 
    uint8_t data = ReadByte(src);
    //Set the Y register to the data 
    YR = data;

    return;
};
void cmos6502::Op_LSR(uint16_t src){
    //Get data from memory
    uint8_t data = ReadByte(src);
    //Set the carry flag if the data is odd
    SetFlag(CARRY, data & 0x01);
    //Shift the data right
    data >>= 1;
    //Write the data back to memory
    WriteByte(src, data);

    return;
};
void cmos6502::Op_LSR_ACC(uint16_t src){
    //Set the carry flag if the accumulator is odd
    SetFlag(CARRY, AC & 0x01);
    //Shift the accumulator right
    AC >>= 1;
    SetFlag(ZERO, !AC);
    SetFlag(NEGATIVE, AC & 0x80);


    return;
};
void cmos6502::Op_NOP(uint16_t src){
    return;
};
void cmos6502::Op_ORA(uint16_t src){
    //Get data from memory
    uint8_t data = ReadByte(src);
    //Set the accumulator to the accumulator OR the data
    AC |= data;
    //Set the zero flag if the accumulator is zero
    SetFlag(ZERO, !AC);
    //Set the negative flag if the accumulator is negative
    SetFlag(NEGATIVE, AC & 0x80);

    return;
};
void cmos6502::Op_PHA(uint16_t src){
    //Push the accumulator to the stack
    StackPush(AC);

    return;
};
void cmos6502::Op_PHP(uint16_t src){
    //Push the status register to the stack
    StackPush(SR | 0x10);

    return;
};
void cmos6502::Op_PLA(uint16_t src){
    //Get data from the stack
    uint8_t data = StackPop();
    //Set the accumulator to the data
    AC = data;

    return;
};
void cmos6502::Op_PLP(uint16_t src){
    //Get the status register from the stack
    uint8_t status = StackPop();
    //Set the status register to the data
    SR = status;

    return;
};
void cmos6502::Op_ROL(uint16_t src){

};
void cmos6502::Op_ROL_ACC(uint16_t src){
    //Get the carry flag
    uint8_t carry = CheckFlag(CARRY);
    //Set the carry flag to the most significant bit of the accumulator
    SetFlag(CARRY, AC & 0x80);
    //Shift the accumulator left
    AC <<= 1;
    //Set the least significant bit of the accumulator to the carry flag
    AC |= carry;
    //Set the zero flag if the accumulator is zero
    SetFlag(ZERO, !AC);
    //Set the negative flag if the accumulator is negative
    SetFlag(NEGATIVE, AC & 0x80);

    return;
};
void cmos6502::Op_ROR(uint16_t src){
    //Get data from memory
    uint8_t data = ReadByte(src);
    //Get the carry flag
    uint8_t carry = CheckFlag(CARRY);
    //Set the carry flag to the least significant bit of the data
    SetFlag(CARRY, data & 0x01);
    //Shift the data right
    data >>= 1;
    //Set the most significant bit of the data to the carry flag
    data |= (carry << 7);
    //Write the data back to memory
    WriteByte(src, data);
    //Set the zero flag if the data is zero
    SetFlag(ZERO, !data);
    //Set the negative flag if the data is negative
    SetFlag(NEGATIVE, data & 0x80);

    return;
};
void cmos6502::Op_ROR_ACC(uint16_t src){

    // Get the carry flag
    uint8_t carry = CheckFlag(CARRY);
    // Set the carry flag to the least significant bit of the accumulator
    SetFlag(CARRY, AC & 0x01);
    // Shift the accumulator right
    AC >>= 1;
    // Set the most significant bit of the accumulator to the carry flag
    AC |= (carry << 7);
    // Set the zero flag if the accumulator is zero
    SetFlag(ZERO, !AC);
    // Set the negative flag if the accumulator is negative
    SetFlag(NEGATIVE, AC & 0x80);

    return;
};
void cmos6502::Op_RTI(uint16_t src){
    //Get the status register from the stack
    uint8_t status = StackPop();
    //Set the status register to the data
    SR = status;
    //Get the return address from the stack 
    uint16_t addr = StackPop();
    addr |= (StackPop() << 8);
    //Set the program counter to the return address
    PC = addr;

    return;
};
void cmos6502::Op_RTS(uint16_t src){
    //Get the return address from the stack 
    uint16_t addr = StackPop();
    addr |= (StackPop() << 8);
    //Set the program counter to the return address
    PC = addr;

    return;
};
void cmos6502::Op_SBC(uint16_t src){
    
    uint8_t data = ReadByte(src);
	unsigned tmp = AC - data - (CheckFlag(CARRY));
    SetFlag(NEGATIVE, tmp & 0x80);
    SetFlag(ZERO, !(tmp & 0xFF));
    SetFlag(OVERFLOW, ((AC ^ tmp) & (AC ^ data)) & 0x80);

    if(CheckFlag(DECIMAL)){ 
        if(((AC & 0xF) - (CheckFlag(CARRY) - 1)) < (data & 0xF)){
            tmp -= 6;
        }
        if(tmp > 0x99){
            tmp -= 0x60;
        }
    }
    SetFlag(CARRY, tmp < 0x100);
    AC = tmp & 0xFF;

    return;
};
void cmos6502::Op_SEC(uint16_t src){
    SetFlag(CARRY, true);
    return;
};
void cmos6502::Op_SED(uint16_t src){
    SetFlag(DECIMAL, true);
    return;
};
void cmos6502::Op_SEI(uint16_t src){
    SetFlag(INTERRUPT, true);
    return;
};
void cmos6502::Op_STA(uint16_t src){
    //Get data from accumulator
    uint8_t data = AC;
    //Write data to memory
    WriteByte(src, data);

    return;
};
void cmos6502::Op_STX(uint16_t src){
    //Get data from accumulator
    uint8_t data = XR;
    //Write data to memory
    WriteByte(src, data);

    return;
};
void cmos6502::Op_STY(uint16_t src){
    //Get data from accumulator
    uint8_t data = YR;
    //Write data to memory
    WriteByte(src, data);

    return;
};
void cmos6502::Op_TAX(uint16_t src){
    //Get data from accumulator
    uint8_t data = AC;
    //Set the X register to the data
    XR = data;

    return;
};
void cmos6502::Op_TAY(uint16_t src){
    //Get data from accumulator 
    uint8_t data = AC;
    //Set the Y register to the data 
    YR = data;

    return;
};
void cmos6502::Op_TSX(uint16_t src){
    //Get data from stack pointer
    uint8_t data = SP;
    //Set the X register to the data
    XR = data;

    return;
};
void cmos6502::Op_TXA(uint16_t src){
    //Get data from X register
    uint8_t data = XR;
    //Set the accumulator to the data
    AC = data;

    return;
};
void cmos6502::Op_TXS(uint16_t src){
    //Get data from X register
    uint8_t data = XR;
    //Set the stack pointer to the data
    SP = data;

    return;
};
void cmos6502::Op_TYA(uint16_t src){
    //Get data from Y register
    uint8_t data = YR;
    //Set the accumulator to the data
    AC = data;

    return;
};
void cmos6502::Op_ILLEGAL(uint16_t src){
    std::cout << "Illegal opcode executed. Halting!!!!!" << std::endl;
    Running = false;
    
    return;
};


uint8_t cmos6502::ReadByte(uint16_t address){
    return Memory[address];
};
void cmos6502::WriteByte(uint16_t address, uint8_t byte){ 
    Memory[address] = byte;
}; 
uint8_t cmos6502::StackPop(){
	if(SP == 0xFF) SP = 0x00;
	else SP++;
	return ReadByte(0x0100 + SP);
}
void cmos6502::StackPush(uint8_t byte){
	WriteByte(0x0100 + SP, byte);
	if(SP == 0x00) SP = 0xFF;
	else SP--;
};
bool cmos6502::CheckFlag(Flags flag) const {
    return (SR & flag) != 0;
};
void cmos6502::SetFlag(Flags flag, bool condition){
    condition ? (SR |= flag) : (SR &= ~flag);
}



void cmos6502::Execute(Instruction i){ 
    (this->*i.code)((this->*i.addr)());
    CycleCount += i.cycles;
    InstructionCount++;
}

void cmos6502::Load(std::vector<uint8_t> &data, uint16_t offset){ 
    if(data.size() + offset > 65536){ 
        std::cout << "Program too large for memory." << std::endl;
        exit(1);
    }
    for(int i = 0; i < data.size(); i++){ 
        Memory[i + offset] = data[i];
    }

    return;

};

void cmos6502::IRQ(){ 
    if(CheckFlag(INTERRUPT) == 0){ 
        StackPush(PC >> 8);
        StackPush(PC & 0xFF);
        StackPush(SR);
        SetFlag(INTERRUPT, true);
        PC = ReadByte(IRQVector) | (ReadByte(IRQVector+1) << 8);
        CycleCount += 7;
    }
};
void cmos6502::NMI(){ 
    StackPush(PC >> 8);
    StackPush(PC & 0xFF);
    StackPush(SR);
    SetFlag(INTERRUPT, true);
    PC = ReadByte(NMIVector) | (ReadByte(NMIVector+1) << 8);
    CycleCount += 8;
};
void cmos6502::RESET(){ 

    // Reset Registers
    AC = 0x00; 
    XR = 0x00;
    YR = 0x00;
    SP = 0xFF;
    SR = 0x36;

    PC = ReadByte(RESETVector) | (ReadByte(RESETVector+1) << 8);

}
void cmos6502::Run(){ 
    std::cout << "Executing Program." << std::endl;

    Running = true;

    while(Running & (PC < 65536)){ 

        uint8_t opcode = ReadByte(PC++);
        Execute(Instructions[opcode]);

    }
};

