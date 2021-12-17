#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <sstream>

#include "Utils.h"

class Bus;

class ARM7TDMI
{
	// addressing mode masks
	/*const static uint32_t AMASK_MUL = 0x00000090;
	const static uint32_t AMASK_MUL_LONG = 0x00800090;
	const static uint32_t AMASK_BRANCH_EXCHANGE = 0x012fff10;
	const static uint32_t AMASK_SWAP = 0x01000090;
	const static uint32_t AMASK_HDT_REGOFFSET = 0x000000b0;
	const static uint32_t AMASK_HDT_IMMOFFSET = 0x004000b0;
	const static uint32_t AMASK_SDT = 0x000000d0;
	const static uint32_t AMASK_DP_PSR = 0;*/

public:
	ARM7TDMI();
	~ARM7TDMI();

	Bus* bus = nullptr;
	void ConnectBus(Bus* n) { bus = n; }

	/*
		Reads from memory in 3 modes: word (32 bit), halfword (16 bit), byte (8bit).
		Default is word.
	*/
	uint32_t read(uint32_t a);
	uint32_t read(uint32_t a, uint32_t size);
	uint16_t read(uint16_t a);
	uint16_t read(uint16_t a, uint16_t size);
	void write(uint32_t a, uint32_t data);
	void write(uint32_t a, uint32_t data, uint32_t size);
	void write(uint16_t a, uint16_t data);

public:
	//cond
	bool EQ(); bool NE(); bool CS(); bool CC(); bool MI();
	bool PL(); bool VS(); bool VC(); bool HI(); bool LS();
	bool GE(); bool LT(); bool GT(); bool LE(); bool AL();
	bool NV(); //never
	struct CONDITION {
		std::string name;
		bool(ARM7TDMI::* cond) (void) = nullptr;
	};
	std::vector<CONDITION> condition_lookup;
	bool conditionPassed();
	
	//addrmodes
	uint32_t m1_IMM_f(); uint32_t m1_REG_f(); uint32_t m1_LLI_f();
	uint32_t m1_LLR_f(); uint32_t m1_LRI_f(); uint32_t m1_LRR_f();
	uint32_t m1_ARI_f(); uint32_t m1_ARR_f(); uint32_t m1_RRI_f();
	uint32_t m1_RRR_f(); uint32_t m1_RRX_f();
	//Mode 2 - Load/Store word/unsigned byte
	uint32_t m2_IMO_f(); uint32_t m2_RGO_f(); uint32_t m2_SRO_f();
	uint32_t m2_PIM_f(); uint32_t m2_PRG_f(); uint32_t m2_PSR_f();
	uint32_t m2_IMP_f(); uint32_t m2_RGP_f(); uint32_t m2_SRP_f();
	//Mode 3 - Misc Load/store
	uint32_t m3_IMO_f(); uint32_t m3_RGO_f(); uint32_t m3_PIM_f();
	uint32_t m3_PRG_f(); uint32_t m3_IMP_f(); uint32_t m3_RGP_f();
	//Mode 4 - Load/store multiple
	uint32_t m4_IA_f(); uint32_t m4_IB_f();
	uint32_t m4_DA_f(); uint32_t m4_DB_f();
	//nothing mode
	uint32_t XXX_f();

	enum AddrModeGroup { AM_MODE1, 
						AM_MODE2,
						AM_MODE3,
						AM_MODE4,
						AM_NOTHING};

	enum class AddrModeName
	{
		m1_IMM, m1_REG, m1_LLI, m1_LLR, m1_LRI, m1_LRR, m1_ARI, m1_ARR, m1_RRI, m1_RRR, m1_RRX,
		m2_IMO, m2_RGO, m2_SRO, m2_PIM, m2_PRG, m2_PSR, m2_IMP, m2_RGP, m2_SRP,
		m3_IMO, m3_RGO, m3_PIM, m3_PRG, m3_IMP, m3_RGP,
		m4_IA, m4_IB, m4_DA, m4_DB,
		XXX
	};

	struct AddrMode {
		AddrModeName name;
		AddrModeGroup group;
		uint32_t(ARM7TDMI::* addrmode_f) (void) = nullptr;
	};

	//Mode 1 - data processing
	AddrMode m1_IMM = { AddrModeName::m1_IMM, AM_MODE1, &ARM7TDMI::m1_IMM_f};
	AddrMode m1_REG = { AddrModeName::m1_REG, AM_MODE1, &ARM7TDMI::m1_REG_f };
	AddrMode m1_LLI = { AddrModeName::m1_LLI, AM_MODE1, &ARM7TDMI::m1_LLI_f };
	AddrMode m1_LLR = { AddrModeName::m1_LLR, AM_MODE1, &ARM7TDMI::m1_LLR_f };
	AddrMode m1_LRI = { AddrModeName::m1_LRI, AM_MODE1, &ARM7TDMI::m1_LRI_f };
	AddrMode m1_LRR = { AddrModeName::m1_LRR, AM_MODE1, &ARM7TDMI::m1_LRR_f };
	AddrMode m1_ARI = { AddrModeName::m1_ARI, AM_MODE1, &ARM7TDMI::m1_ARI_f };
	AddrMode m1_ARR = { AddrModeName::m1_ARR, AM_MODE1, &ARM7TDMI::m1_ARR_f };
	AddrMode m1_RRI = { AddrModeName::m1_RRI, AM_MODE1, &ARM7TDMI::m1_RRI_f };
	AddrMode m1_RRR = { AddrModeName::m1_RRR, AM_MODE1, &ARM7TDMI::m1_RRR_f };
	AddrMode m1_RRX = { AddrModeName::m1_RRX, AM_MODE1, &ARM7TDMI::m1_RRX_f };
	//Mode 2 - Load/Store word/unsigned byte
	AddrMode m2_IMO = { AddrModeName::m2_IMO, AM_MODE2, &ARM7TDMI::m2_IMO_f };
	AddrMode m2_RGO = { AddrModeName::m2_RGO, AM_MODE2, &ARM7TDMI::m2_RGO_f };
	AddrMode m2_SRO = { AddrModeName::m2_SRO, AM_MODE2, &ARM7TDMI::m2_SRO_f };
	AddrMode m2_PIM = { AddrModeName::m2_PIM, AM_MODE2, &ARM7TDMI::m2_PIM_f };
	AddrMode m2_PRG = { AddrModeName::m2_PRG, AM_MODE2, &ARM7TDMI::m2_PRG_f };
	AddrMode m2_PSR = { AddrModeName::m2_PSR, AM_MODE2, &ARM7TDMI::m2_PSR_f };
	AddrMode m2_IMP = { AddrModeName::m2_IMP, AM_MODE2, &ARM7TDMI::m2_IMP_f };
	AddrMode m2_RGP = { AddrModeName::m2_RGP, AM_MODE2, &ARM7TDMI::m2_RGP_f };
	AddrMode m2_SRP = { AddrModeName::m2_SRP, AM_MODE2, &ARM7TDMI::m2_SRP_f };
	//Mode 3 - Misc Load/store
	AddrMode m3_IMO = { AddrModeName::m3_IMO, AM_MODE3, &ARM7TDMI::m3_IMO_f };
	AddrMode m3_RGO = { AddrModeName::m3_RGO, AM_MODE3, &ARM7TDMI::m3_RGO_f };
	AddrMode m3_PIM = { AddrModeName::m3_PIM, AM_MODE3, &ARM7TDMI::m3_PIM_f };
	AddrMode m3_PRG = { AddrModeName::m3_PRG, AM_MODE3, &ARM7TDMI::m3_PRG_f };
	AddrMode m3_IMP = { AddrModeName::m3_IMP, AM_MODE3, &ARM7TDMI::m3_IMP_f };
	AddrMode m3_RGP = { AddrModeName::m3_RGP, AM_MODE3, &ARM7TDMI::m3_RGP_f };
	//Mode 4 - Load/store multiple
	AddrMode m4_IA = { AddrModeName::m4_IA, AM_MODE4, &ARM7TDMI::m4_IA_f };
	AddrMode m4_IB = { AddrModeName::m4_IB, AM_MODE4, &ARM7TDMI::m4_IB_f };
	AddrMode m4_DA = { AddrModeName::m4_DA, AM_MODE4, &ARM7TDMI::m4_DA_f };
	AddrMode m4_DB = { AddrModeName::m4_DB, AM_MODE4, &ARM7TDMI::m4_DB_f };

	AddrMode XXX = { AddrModeName::XXX, AM_NOTHING, &ARM7TDMI::XXX_f };

	//ARM instructions
	//Move
	uint32_t MOV(); uint32_t MVN(); 
	uint32_t MRS(); uint32_t MSR();
	//Arithmetic
	uint32_t ADD(); uint32_t ADC(); 
	uint32_t SUB(); uint32_t SBC();
	uint32_t RSB(); uint32_t RSC(); 
	//Logical
	uint32_t CMP(); uint32_t CMN();
	uint32_t TST(); uint32_t TEQ(); 
	uint32_t AND(); uint32_t EOR(); 
	uint32_t ORR(); uint32_t BIC();
	//Multiply
	uint32_t MUL(); uint32_t MLA();
	uint32_t UMULL(); uint32_t UMLAL();
	uint32_t SMULL(); uint32_t SMLAL();
	//Branch
	uint32_t B();	uint32_t BX();
	//Load/store
	uint32_t LDR(); uint32_t LDRB(); uint32_t LDRBT(); uint32_t LDRT();
	uint32_t LDRH(); uint32_t LDRSB(); uint32_t LDRSH();
	uint32_t LDM_1(); uint32_t LDM_2(); uint32_t LDM_3();
	uint32_t STR(); uint32_t STRT(); uint32_t STRB(); uint32_t STRBT();
	uint32_t STRH(); uint32_t STM_1(); uint32_t STM_2();
	//Swap
	uint32_t SWP(); uint32_t SWPB();
	//Coprocessor
	uint32_t CDP(); uint32_t MRC(); uint32_t MCR(); 
	uint32_t LDC(); uint32_t STC();
	//Software interrupt
	uint32_t SWI();

	//nothing
	uint32_t NOP();

public:
	std::array<uint32_t, 12> r; //r0-r12
	uint32_t sp = 0; //r13 - Stack Pointer
	uint32_t lr = 0; //r14 - Link Ptr
	uint32_t pc = 0; //r15 - Prog Counter

	std::array<uint32_t, 5> r_fiq; //banked registers of fiq mode
	std::array<uint32_t, 5> sp_banked; //banked r13, access by modes 2-6
	std::array<uint32_t, 5> lr_banked; //banked r14, access by modes 2-6
	
	/*Modes:
		0 - User
		1 - System
		2 - svc
		3 -	abt
		4 - und
		5 - irq
		6 - fiq*/
	uint8_t mode = 0;

	uint32_t readRegister(uint32_t n, uint8_t force_mode);
	uint32_t readRegister(uint32_t n);
	void writeRegister(uint32_t n, uint32_t data);
	void writeRegister(uint32_t n, uint32_t data, uint8_t force_mode);


	std::string getRegisterName(uint32_t);

	//std::array<uint32_t, 5> spsr; //Saved Prog Status Register
	union ProgStatReg {
		struct
		{
			uint32_t M : 5;  //mode
			uint32_t T : 1;  //Thumb
			uint32_t F : 1;  //FIQ
			uint32_t I : 1;  //IRQ
			uint32_t R : 20; //reserved
			uint32_t V : 1;  //oVerflow
			uint32_t C : 1;  //carry
			uint32_t Z : 1;  //zero
			uint32_t N : 1;  //negative
		};

		uint32_t reg;
	};

	ProgStatReg cpsr; //Current Prog Status Register

	std::array<ProgStatReg, 5> spsr; //banked spsr, access by modes 2-6
	ProgStatReg getSPSR();
	void setSPSR(uint32_t data); //set the whole 32-bit spsr, not individual bit

	uint32_t prefetch = 0;
	uint32_t opcode = 0x0000;
	uint32_t cycles = 0;

	struct Instruction {
		std::string name = "";
		uint32_t(ARM7TDMI::* operate) (void) = nullptr;
		//uint32_t(ARM7TDMI::* addrmode) (void) = nullptr;
		AddrMode *addrmode = nullptr;
	};
	std::vector<std::vector<Instruction>> instruction_lookup;
	
	uint32_t shifter_operand;
	uint32_t shifter_carry_out;

	//mode 2 & 3
	uint32_t ls_addr;

	//mode 4
	uint32_t ls_start_addr;
	uint32_t ls_end_addr;

public:
	void disassembleARM(const std::vector<uint32_t> &mem);
	std::string disassembleARMInstruction(const uint32_t instruction);

private:
	std::string parseShiftIMM(const uint32_t instruction);
};

