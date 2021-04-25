#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <array>

class ARM7TDMI
{
public:
	ARM7TDMI();
	~ARM7TDMI();

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
	std::vector<CONDITION> cond_lookup;
	
	//addrmodes
	//Mode 1 - data processing
	uint32_t m1_IMM(); uint32_t m1_REG(); uint32_t m1_LLI();
	uint32_t m1_LLR(); uint32_t m1_LRI(); uint32_t m1_LRR();
	uint32_t m1_ARI(); uint32_t m1_ARR(); uint32_t m1_RRI();
	uint32_t m1_RRR(); uint32_t m1_RRX();
	//Mode 2 - Load/Store word/unsigned byte
	uint32_t m2_IMO(); uint32_t m2_RGO(); uint32_t m2_SRO();
	uint32_t m2_PIM(); uint32_t m2_PRG(); uint32_t m2_PSR();
	uint32_t m2_IMP(); uint32_t m2_RGP(); uint32_t m2_SRP();
	//Mode 3 - Misc Load/store
	uint32_t m3_IMO(); uint32_t m3_RGO(); uint32_t m3_PIM(); 
	uint32_t m3_PRG(); uint32_t m3_IMP(); uint32_t m3_RGP();
	//Mode 4 - Load/store multiple
	uint32_t m4_IA(); uint32_t m4_IB();
	uint32_t m4_DA(); uint32_t m4_DB();

	//nothing mode
	uint32_t XXX();

	//ARM instructions
	//Move
	uint32_t MOV(); uint32_t MVN(); uint32_t MRS(); uint32_t MSR();
	//Arithmetic
	uint32_t ADD(); uint32_t ADC(); uint32_t SUB(); uint32_t SBC();
	uint32_t RSB(); uint32_t RSC(); uint32_t MUL(); uint32_t MLA();
	uint32_t UMULL(); uint32_t UMLAL();
	uint32_t SMULL(); uint32_t SMLAL();
	uint32_t CMP(); uint32_t CMN();
	//Logical
	uint32_t TST(); uint32_t TEQ(); uint32_t AND(); 
	uint32_t EOR(); uint32_t ORR(); uint32_t BIC();
	//Branch
	uint32_t B();	uint32_t BL();	uint32_t BX();
	//Load/store
	uint32_t LDR(); uint32_t LDRB(); uint32_t LDRBT(); uint32_t LDRT();
	uint32_t LDRH(); uint32_t LDRSB(); uint32_t LDRSH();
	uint32_t LDMIB(); uint32_t LDMIA(); uint32_t LDMDB();
	uint32_t LDMDA();
	uint32_t LDM_1(); uint32_t LDM_2(); uint32_t LDM_3();
	uint32_t STR(); uint32_t STRT(); uint32_t STRB(); uint32_t STRBT();
	uint32_t STRH(); uint32_t STMIB(); uint32_t STMIA(); uint32_t STMDB();
	uint32_t STMDA(); uint32_t STM_1(); uint32_t STM_2();
	//Swap
	uint32_t SWP(); uint32_t SWPB();
	//Coprocessor
	uint32_t CDP(); uint32_t MRC(); uint32_t MCR(); 
	uint32_t LDC(); uint32_t STC();
	//Software interrupt
	uint32_t SWI();

	//nothing
	uint32_t NOP();

	std::array<uint32_t, 12> r;//r0-r12
	uint32_t sp; //r13 - Stack Pointer
	uint32_t lr; //r14 - Link Ptr
	uint32_t pc; //r15 - Prog Counter
	//uint32_t cpsr; //Current Prog Status Register
	//std::array<uint32_t, 5> spsr; //Saved Prog Status Register
	union {
		struct
		{
			unsigned N : 1;
			unsigned Z : 1;
			unsigned C : 1;
			unsigned V : 1;
			unsigned R : 20;
			unsigned I : 1;
			unsigned F : 1;
			unsigned T : 1;
			unsigned M : 5;
		};

		uint32_t reg;
	} cpsr;

	

	struct INSTRUCTION {
		std::string name = "";
		uint32_t(ARM7TDMI::* operate) (void) = nullptr;
		uint32_t(ARM7TDMI::* addrmode) (void) = nullptr;
	};
	std::vector<std::vector<INSTRUCTION>> lookup;

	bool conditionPassed();

};

