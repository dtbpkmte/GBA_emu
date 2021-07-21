#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <bitset>

#include "Utils.h"

class Bus;

class ARM7TDMI
{
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

	enum ADDRMODE_TYPE {AM_MODE1, 
						AM_MODE2,
						AM_MODE3,
						AM_MODE4,
						AM_NOTHING};
	struct ADDRMODE {
		ADDRMODE_TYPE type;
		uint32_t(ARM7TDMI::* addrmode_f) (void) = nullptr;
	};

	//Mode 1 - data processing
	ADDRMODE m1_IMM = { AM_MODE1, &ARM7TDMI::m1_IMM_f };
	ADDRMODE m1_REG = { AM_MODE1, &ARM7TDMI::m1_REG_f };
	ADDRMODE m1_LLI = { AM_MODE1, &ARM7TDMI::m1_LLI_f };
	ADDRMODE m1_LLR = { AM_MODE1, &ARM7TDMI::m1_LLR_f };
	ADDRMODE m1_LRI = { AM_MODE1, &ARM7TDMI::m1_LRI_f };
	ADDRMODE m1_LRR = { AM_MODE1, &ARM7TDMI::m1_LRR_f };
	ADDRMODE m1_ARI = { AM_MODE1, &ARM7TDMI::m1_ARI_f };
	ADDRMODE m1_ARR = { AM_MODE1, &ARM7TDMI::m1_ARR_f };
	ADDRMODE m1_RRI = { AM_MODE1, &ARM7TDMI::m1_RRI_f };
	ADDRMODE m1_RRR = { AM_MODE1, &ARM7TDMI::m1_RRR_f };
	ADDRMODE m1_RRX = { AM_MODE1, &ARM7TDMI::m1_RRX_f };
	//Mode 2 - Load/Store word/unsigned byte
	ADDRMODE m2_IMO = { AM_MODE2, &ARM7TDMI::m2_IMO_f };
	ADDRMODE m2_RGO = { AM_MODE2, &ARM7TDMI::m2_RGO_f };
	ADDRMODE m2_SRO = { AM_MODE2, &ARM7TDMI::m2_SRO_f };
	ADDRMODE m2_PIM = { AM_MODE2, &ARM7TDMI::m2_PIM_f };
	ADDRMODE m2_PRG = { AM_MODE2, &ARM7TDMI::m2_PRG_f };
	ADDRMODE m2_PSR = { AM_MODE2, &ARM7TDMI::m2_PSR_f };
	ADDRMODE m2_IMP = { AM_MODE2, &ARM7TDMI::m2_IMP_f };
	ADDRMODE m2_RGP = { AM_MODE2, &ARM7TDMI::m2_RGP_f };
	ADDRMODE m2_SRP = { AM_MODE2, &ARM7TDMI::m2_SRP_f };
	//Mode 3 - Misc Load/store
	ADDRMODE m3_IMO = { AM_MODE3, &ARM7TDMI::m3_IMO_f };
	ADDRMODE m3_RGO = { AM_MODE3, &ARM7TDMI::m3_RGO_f };
	ADDRMODE m3_PIM = { AM_MODE3, &ARM7TDMI::m3_PIM_f };
	ADDRMODE m3_PRG = { AM_MODE3, &ARM7TDMI::m3_PRG_f };
	ADDRMODE m3_IMP = { AM_MODE3, &ARM7TDMI::m3_IMP_f };
	ADDRMODE m3_RGP = { AM_MODE3, &ARM7TDMI::m3_RGP_f };
	//Mode 4 - Load/store multiple
	ADDRMODE m4_IA = { AM_MODE4, &ARM7TDMI::m4_IA_f };
	ADDRMODE m4_IB = { AM_MODE4, &ARM7TDMI::m4_IB_f };
	ADDRMODE m4_DA = { AM_MODE4, &ARM7TDMI::m4_DA_f };
	ADDRMODE m4_DB = { AM_MODE4, &ARM7TDMI::m4_DB_f };

	ADDRMODE XXX = { AM_NOTHING, &ARM7TDMI::XXX_f };

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

	struct INSTRUCTION {
		std::string name = "";
		uint32_t(ARM7TDMI::* operate) (void) = nullptr;
		//uint32_t(ARM7TDMI::* addrmode) (void) = nullptr;
		ADDRMODE *addrmode = nullptr;
	};
	std::vector<std::vector<INSTRUCTION>> instruction_lookup;
	
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
	std::string parseOperand2(const uint32_t instruction);
};

