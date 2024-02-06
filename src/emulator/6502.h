#include <stdint.h>
#include <iostream>
#include <vector>

class cmos6502 { 
    private:
        uint16_t PC; 
        uint8_t SP;
        uint8_t SR; 
        uint8_t AC; 
        uint8_t XR; 
        uint8_t YR;
        uint8_t Memory[65536];
		bool Running;

		int InstructionCount;
		int CycleCount;

		// Instructions

        // Instruction decode and execution functions
        typedef void (cmos6502::*CodeExec)(uint16_t);
	    typedef uint16_t (cmos6502::*AddrExec)();
		
		struct Instruction {
			CodeExec code;
			AddrExec addr;
			uint8_t cycles;
		};

		void Execute(Instruction i);

		uint16_t Add_ACC();
		uint16_t Add_IMM();
		uint16_t Add_ABS();
		uint16_t Add_ZER();
		uint16_t Add_ZEX();
		uint16_t Add_ZEY();
		uint16_t Add_ABX();
		uint16_t Add_ABY();
		uint16_t Add_IMP();
		uint16_t Add_REL();
		uint16_t Add_INX();
		uint16_t Add_INY();
		uint16_t Add_ABI();

		void Op_ADC(uint16_t src);
		void Op_AND(uint16_t src);
		void Op_ASL(uint16_t src); 	
		void Op_ASL_ACC(uint16_t src);
		void Op_BCC(uint16_t src);
		void Op_BCS(uint16_t src);

		void Op_BEQ(uint16_t src);
		void Op_BIT(uint16_t src);
		void Op_BMI(uint16_t src);
		void Op_BNE(uint16_t src);
		void Op_BPL(uint16_t src);

		void Op_BRK(uint16_t src);
		void Op_BVC(uint16_t src);
		void Op_BVS(uint16_t src);
		void Op_CLC(uint16_t src);
		void Op_CLD(uint16_t src);

		void Op_CLI(uint16_t src);
		void Op_CLV(uint16_t src);
		void Op_CMP(uint16_t src);
		void Op_CPX(uint16_t src);
		void Op_CPY(uint16_t src);

		void Op_DEC(uint16_t src);
		void Op_DEX(uint16_t src);
		void Op_DEY(uint16_t src);
		void Op_EOR(uint16_t src);
		void Op_INC(uint16_t src);

		void Op_INX(uint16_t src);
		void Op_INY(uint16_t src);
		void Op_JMP(uint16_t src);
		void Op_JSR(uint16_t src);
		void Op_LDA(uint16_t src);

		void Op_LDX(uint16_t src);
		void Op_LDY(uint16_t src);
		void Op_LSR(uint16_t src); 	
		void Op_LSR_ACC(uint16_t src);
		void Op_NOP(uint16_t src);
		void Op_ORA(uint16_t src);

		void Op_PHA(uint16_t src);
		void Op_PHP(uint16_t src);
		void Op_PLA(uint16_t src);
		void Op_PLP(uint16_t src);
		void Op_ROL(uint16_t src); 	
		void Op_ROL_ACC(uint16_t src);

		void Op_ROR(uint16_t src);	
		void Op_ROR_ACC(uint16_t src);
		void Op_RTI(uint16_t src);
		void Op_RTS(uint16_t src);
		void Op_SBC(uint16_t src);
		void Op_SEC(uint16_t src);
		void Op_SED(uint16_t src);

		void Op_SEI(uint16_t src);
		void Op_STA(uint16_t src);
		void Op_STX(uint16_t src);
		void Op_STY(uint16_t src);
		void Op_TAX(uint16_t src);

		void Op_TAY(uint16_t src);
		void Op_TSX(uint16_t src);
		void Op_TXA(uint16_t src);
		void Op_TXS(uint16_t src);
		void Op_TYA(uint16_t src);

		void Op_ILLEGAL(uint16_t src);

        Instruction Instructions[256] = { 
			// define all opcodes
			{&cmos6502::Op_BRK, &cmos6502::Add_IMP, 7},
			{&cmos6502::Op_ORA, &cmos6502::Add_INX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 3}, 
			{&cmos6502::Op_ORA, &cmos6502::Add_ZER, 3}, 
			{&cmos6502::Op_ASL, &cmos6502::Add_ZER, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5}, 
			{&cmos6502::Op_PHP, &cmos6502::Add_IMP, 3}, 
			{&cmos6502::Op_ORA, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_ASL_ACC, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_ORA, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_ASL, &cmos6502::Add_ABS, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_BPL, &cmos6502::Add_REL, 2},
			{&cmos6502::Op_ORA, &cmos6502::Add_INY, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4}, 
			{&cmos6502::Op_ORA, &cmos6502::Add_ZEX, 4}, 
			{&cmos6502::Op_ASL, &cmos6502::Add_ZEX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6}, 
			{&cmos6502::Op_CLC, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ORA, &cmos6502::Add_ABY, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4}, 
			{&cmos6502::Op_ORA, &cmos6502::Add_ABX, 4}, 
			{&cmos6502::Op_ASL, &cmos6502::Add_ABX, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7}, 
			{&cmos6502::Op_JSR, &cmos6502::Add_ABS, 6},
			{&cmos6502::Op_AND, &cmos6502::Add_INX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8}, 
			{&cmos6502::Op_BIT, &cmos6502::Add_ZER, 3}, 
			{&cmos6502::Op_AND, &cmos6502::Add_ZER, 3}, 
			{&cmos6502::Op_ROL, &cmos6502::Add_ZER, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5}, 
			{&cmos6502::Op_PLP, &cmos6502::Add_IMP, 4}, 
			{&cmos6502::Op_AND, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_ROL_ACC, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_BIT, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_AND, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_ROL, &cmos6502::Add_ABS, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_BMI, &cmos6502::Add_REL, 2},
			{&cmos6502::Op_AND, &cmos6502::Add_INY, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4}, 
			{&cmos6502::Op_AND, &cmos6502::Add_ZEX, 4}, 
			{&cmos6502::Op_ROL, &cmos6502::Add_ZEX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6}, 
			{&cmos6502::Op_SEC, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_AND, &cmos6502::Add_ABY, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4}, 
			{&cmos6502::Op_AND, &cmos6502::Add_ABX, 4}, 
			{&cmos6502::Op_ROL, &cmos6502::Add_ABX, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7}, 
			{&cmos6502::Op_RTI, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_EOR, &cmos6502::Add_INX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 3}, 
			{&cmos6502::Op_EOR, &cmos6502::Add_ZER, 3}, 
			{&cmos6502::Op_LSR, &cmos6502::Add_ZER, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5}, 
			{&cmos6502::Op_PHA, &cmos6502::Add_IMP, 3}, 
			{&cmos6502::Op_EOR, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_LSR_ACC, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_JMP, &cmos6502::Add_ABS, 3},
			{&cmos6502::Op_EOR, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_LSR, &cmos6502::Add_ABS, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6 },
			{&cmos6502::Op_BVC, &cmos6502::Add_REL, 2},
			{&cmos6502::Op_EOR, &cmos6502::Add_INY, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4}, 
			{&cmos6502::Op_EOR, &cmos6502::Add_ZEX, 4}, 
			{&cmos6502::Op_LSR, &cmos6502::Add_ZEX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6}, 
			{&cmos6502::Op_CLI, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_EOR, &cmos6502::Add_ABY, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7}, 
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4}, 
			{&cmos6502::Op_EOR, &cmos6502::Add_ABX, 4}, 
			{&cmos6502::Op_LSR, &cmos6502::Add_ABX, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7}, 
			{&cmos6502::Op_RTS, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_ADC, &cmos6502::Add_INX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 3},
			{&cmos6502::Op_ADC, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_ROR, &cmos6502::Add_ZER, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5},
			{&cmos6502::Op_PLA, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_ADC, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_ROR_ACC, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_JMP, &cmos6502::Add_ABI, 6},
			{&cmos6502::Op_ADC, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_ROR, &cmos6502::Add_ABS, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_BVS, &cmos6502::Add_REL, 2},
			{&cmos6502::Op_ADC, &cmos6502::Add_INY, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_ADC, &cmos6502::Add_ZEX, 4},
			{&cmos6502::Op_ROR, &cmos6502::Add_ZEX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_SEI, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ADC, &cmos6502::Add_ABY, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_ADC, &cmos6502::Add_ABX, 4},
			{&cmos6502::Op_ROR, &cmos6502::Add_ABX, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_STA, &cmos6502::Add_INX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_STY, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_STA, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_STX, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 3},
			{&cmos6502::Op_DEY, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_TXA, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_STY, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_STA, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_STX, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_BCC, &cmos6502::Add_REL, 2},
			{&cmos6502::Op_STA, &cmos6502::Add_INY, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_STY, &cmos6502::Add_ZEX, 4},
			{&cmos6502::Op_STA, &cmos6502::Add_ZEX, 4},
			{&cmos6502::Op_STX, &cmos6502::Add_ZEY, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_TYA, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_STA, &cmos6502::Add_ABY, 5},
			{&cmos6502::Op_TXS, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5},
			{&cmos6502::Op_STA, &cmos6502::Add_ABX, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5},
			{&cmos6502::Op_LDY, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_LDA, &cmos6502::Add_INX, 6},
			{&cmos6502::Op_LDX, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_LDY, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_LDA, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_LDX, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 3},
			{&cmos6502::Op_TAY, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_LDA, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_TAX, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_LDY, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_LDA, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_LDX, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_BCS, &cmos6502::Add_REL, 2},
			{&cmos6502::Op_LDA, &cmos6502::Add_INY, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5},
			{&cmos6502::Op_LDY, &cmos6502::Add_ZEX, 4},
			{&cmos6502::Op_LDA, &cmos6502::Add_ZEX, 4},
			{&cmos6502::Op_LDX, &cmos6502::Add_ZEY, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_CLV, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_LDA, &cmos6502::Add_ABY, 4},
			{&cmos6502::Op_TSX, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_LDY, &cmos6502::Add_ABX, 4},
			{&cmos6502::Op_LDA, &cmos6502::Add_ABX, 4},
			{&cmos6502::Op_LDX, &cmos6502::Add_ABY, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_CPY, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_CMP, &cmos6502::Add_INX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8},
			{&cmos6502::Op_CPY, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_CMP, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_DEC, &cmos6502::Add_ZER, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5},
			{&cmos6502::Op_INY, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_CMP, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_DEX, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_CPY, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_CMP, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_DEC, &cmos6502::Add_ABS, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_BNE, &cmos6502::Add_REL, 2},
			{&cmos6502::Op_CMP, &cmos6502::Add_INY, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_CMP, &cmos6502::Add_ZEX, 4},
			{&cmos6502::Op_DEC, &cmos6502::Add_ZEX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_CLD, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_CMP, &cmos6502::Add_ABY, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_CMP, &cmos6502::Add_ABX, 4},
			{&cmos6502::Op_DEC, &cmos6502::Add_ABX, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7},
			{&cmos6502::Op_CPX, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_SBC, &cmos6502::Add_INX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8},
			{&cmos6502::Op_CPX, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_SBC, &cmos6502::Add_ZER, 3},
			{&cmos6502::Op_INC, &cmos6502::Add_ZER, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 5},
			{&cmos6502::Op_INX, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_SBC, &cmos6502::Add_IMM, 2},
			{&cmos6502::Op_NOP, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_CPX, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_SBC, &cmos6502::Add_ABS, 4},
			{&cmos6502::Op_INC, &cmos6502::Add_ABS, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_BEQ, &cmos6502::Add_REL, 2},
			{&cmos6502::Op_SBC, &cmos6502::Add_INY, 5},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 8},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_SBC, &cmos6502::Add_ZEX, 4},
			{&cmos6502::Op_INC, &cmos6502::Add_ZEX, 6},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 6},
			{&cmos6502::Op_SED, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_SBC, &cmos6502::Add_ABY, 4},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 2},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 4},
			{&cmos6502::Op_SBC, &cmos6502::Add_ABX, 4},
			{&cmos6502::Op_INC, &cmos6502::Add_ABX, 7},
			{&cmos6502::Op_ILLEGAL, &cmos6502::Add_IMP, 7} 
		};
		
		enum Flags : uint8_t { 
            NEGATIVE  = 0x80,
            OVERFLOW  = 0x40,
            BREAK     = 0x10,
            DECIMAL   = 0x08,
            INTERRUPT = 0x04,
            ZERO      = 0x02,
            CARRY     = 0x01
        };
	
	public:

		
        cmos6502();
		
        uint8_t ReadByte(uint16_t address); 
        void WriteByte(uint16_t address, uint8_t byte);
        uint8_t StackPop(); 
        void StackPush(uint8_t byte);
        bool CheckFlag(Flags flag) const;
		void SetFlag(Flags flag, bool condition);


        void Load(std::vector<uint8_t> &data, uint16_t offset);
		void IRQ();
		void NMI();
        void Run(); 
        void RESET();


};
