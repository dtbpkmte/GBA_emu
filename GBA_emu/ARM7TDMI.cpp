#include "ARM7TDMI.h"
#include "Bus.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

ARM7TDMI::ARM7TDMI() {
	using a = ARM7TDMI;
	condition_lookup = {
		{"EQ", &a::EQ}, {"NE", &a::NE}, {"CS", &a::CS}, {"CC", &a::CC}, {"MI", &a::MI},
		{"PL", &a::PL}, {"VS", &a::VS}, {"VC", &a::VC}, {"HI", &a::HI}, {"LS", &a::LS}, 
		{"GE", &a::GE}, {"LT", &a::LT}, {"GT", &a::GT}, {"LE", &a::LE}, {"AL", &a::AL},
		{"NV", &a::NV},
	};

	instruction_lookup = {
		{{"AND", &a::AND, &m1_LLI},  {"AND", &a::AND, &m1_LLR},  {"AND", &a::AND, &m1_LRI},  {"AND", &a::AND, &m1_LRR},  {"AND", &a::AND, &m1_ARI},  {"AND", &a::AND, &m1_ARR},  {"AND", &a::AND, &m1_RRI},  {"AND", &a::AND, &m1_RRR},  {"AND", &a::AND, &m1_LLI},  {"MUL", &a::MUL, &XXX},      {"AND", &a::AND, &m1_LRI},  {"STRH", &a::STRH, &m3_RGP}, {"AND", &a::AND, &m1_ARI},  {"???", &a::NOP, &XXX},        {"AND", &a::AND, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"ANDS", &a::AND, &m1_LLI}, {"ANDS", &a::AND, &m1_LLR}, {"ANDS", &a::AND, &m1_LRI}, {"ANDS", &a::AND, &m1_LRR}, {"ANDS", &a::AND, &m1_ARI}, {"ANDS", &a::AND, &m1_ARR}, {"ANDS", &a::AND, &m1_RRI}, {"ANDS", &a::AND, &m1_RRR}, {"ANDS", &a::AND, &m1_LLI}, {"MULS", &a::MUL, &XXX},     {"ANDS", &a::AND, &m1_LRI}, {"LDRH", &a::LDRH, &m3_RGP}, {"ANDS", &a::AND, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_RGP}, {"ANDS", &a::AND, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_RGP}},
		{{"EOR", &a::EOR, &m1_LLI},  {"EOR", &a::EOR, &m1_LLR},  {"EOR", &a::EOR, &m1_LRI},  {"EOR", &a::EOR, &m1_LRR},  {"EOR", &a::EOR, &m1_ARI},  {"EOR", &a::EOR, &m1_ARR},  {"EOR", &a::EOR, &m1_RRI},  {"EOR", &a::EOR, &m1_RRR},  {"EOR", &a::EOR, &m1_LLI},  {"MLA", &a::MLA, &XXX},      {"EOR", &a::EOR, &m1_LRI},  {"STRH", &a::STRH, &m3_RGP}, {"EOR", &a::EOR, &m1_ARI},  {"???", &a::NOP, &XXX},        {"EOR", &a::EOR, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"EORS", &a::EOR, &m1_LLI}, {"EORS", &a::EOR, &m1_LLR}, {"EORS", &a::EOR, &m1_LRI}, {"EORS", &a::EOR, &m1_LRR}, {"EORS", &a::EOR, &m1_ARI}, {"EORS", &a::EOR, &m1_ARR}, {"EORS", &a::EOR, &m1_RRI}, {"EORS", &a::EOR, &m1_RRR}, {"EORS", &a::EOR, &m1_LLI}, {"MLAS", &a::MUL, &XXX},     {"EORS", &a::EOR, &m1_LRI}, {"LDRH", &a::LDRH, &m3_RGP}, {"EORS", &a::EOR, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_RGP}, {"EORS", &a::EOR, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_RGP}},
		{{"SUB", &a::SUB, &m1_LLI},  {"SUB", &a::SUB, &m1_LLR},  {"SUB", &a::SUB, &m1_LRI},  {"SUB", &a::SUB, &m1_LRR},  {"SUB", &a::SUB, &m1_ARI},  {"SUB", &a::SUB, &m1_ARR},  {"SUB", &a::SUB, &m1_RRI},  {"SUB", &a::SUB, &m1_RRR},  {"SUB", &a::SUB, &m1_LLI},  {"???", &a::NOP, &XXX},      {"SUB", &a::SUB, &m1_LRI},  {"STRH", &a::STRH, &m3_IMP}, {"SUB", &a::SUB, &m1_ARI},  {"???", &a::NOP, &XXX},        {"SUB", &a::SUB, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"SUBS", &a::SUB, &m1_LLI}, {"SUBS", &a::SUB, &m1_LLR}, {"SUBS", &a::SUB, &m1_LRI}, {"SUBS", &a::SUB, &m1_LRR}, {"SUBS", &a::SUB, &m1_ARI}, {"SUBS", &a::SUB, &m1_ARR}, {"SUBS", &a::SUB, &m1_RRI}, {"SUBS", &a::SUB, &m1_RRR}, {"SUBS", &a::SUB, &m1_LLI}, {"???", &a::NOP, &XXX},      {"SUBS", &a::SUB, &m1_LRI}, {"LDRH", &a::LDRH, &m3_IMP}, {"SUBS", &a::SUB, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_IMP}, {"SUBS", &a::SUB, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_IMP}},
		{{"RSB", &a::RSB, &m1_LLI},  {"RSB", &a::RSB, &m1_LLR},  {"RSB", &a::RSB, &m1_LRI},  {"RSB", &a::RSB, &m1_LRR},  {"RSB", &a::RSB, &m1_ARI},  {"RSB", &a::RSB, &m1_ARR},  {"RSB", &a::RSB, &m1_RRI},  {"RSB", &a::RSB, &m1_RRR},  {"RSB", &a::RSB, &m1_LLI},  {"???", &a::NOP, &XXX},	    {"RSB", &a::RSB, &m1_LRI},  {"STRH", &a::STRH, &m3_IMP}, {"RSB", &a::RSB, &m1_ARI},  {"???", &a::NOP, &XXX},        {"RSB", &a::RSB, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"RSBS", &a::RSB, &m1_LLI}, {"RSBS", &a::RSB, &m1_LLR}, {"RSBS", &a::RSB, &m1_LRI}, {"RSBS", &a::RSB, &m1_LRR}, {"RSBS", &a::RSB, &m1_ARI}, {"RSBS", &a::RSB, &m1_ARR}, {"RSBS", &a::RSB, &m1_RRI}, {"RSBS", &a::RSB, &m1_RRR}, {"RSBS", &a::RSB, &m1_LLI}, {"???", &a::NOP, &XXX},      {"RSBS", &a::RSB, &m1_LRI}, {"LDRH", &a::LDRH, &m3_IMP}, {"RSBS", &a::RSB, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_IMP}, {"RSBS", &a::RSB, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_IMP}},
		{{"ADD", &a::ADD, &m1_LLI},  {"ADD", &a::ADD, &m1_LLR},  {"ADD", &a::ADD, &m1_LRI},  {"ADD", &a::ADD, &m1_LRR},  {"ADD", &a::ADD, &m1_ARI},  {"ADD", &a::ADD, &m1_ARR},  {"ADD", &a::ADD, &m1_RRI},  {"ADD", &a::ADD, &m1_RRR},  {"ADD", &a::ADD, &m1_LLI},  {"UMULL", &a::UMULL, &XXX},  {"ADD", &a::ADD, &m1_LRI},  {"STRH", &a::STRH, &m3_RGP}, {"ADD", &a::ADD, &m1_ARI},  {"???", &a::NOP, &XXX},        {"ADD", &a::ADD, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"ADDS", &a::ADD, &m1_LLI}, {"ADDS", &a::ADD, &m1_LLR}, {"ADDS", &a::ADD, &m1_LRI}, {"ADDS", &a::ADD, &m1_LRR}, {"ADDS", &a::ADD, &m1_ARI}, {"ADDS", &a::ADD, &m1_ARR}, {"ADDS", &a::ADD, &m1_RRI}, {"ADDS", &a::ADD, &m1_RRR}, {"ADDS", &a::ADD, &m1_LLI}, {"UMULLS", &a::UMULL, &XXX}, {"ADDS", &a::ADD, &m1_LRI}, {"LDRH", &a::LDRH, &m3_RGP}, {"ADDS", &a::ADD, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_RGP}, {"ADDS", &a::ADD, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_RGP}},
		{{"ADC", &a::ADC, &m1_LLI},  {"ADC", &a::ADC, &m1_LLR},  {"ADC", &a::ADC, &m1_LRI},  {"ADC", &a::ADC, &m1_LRR},  {"ADC", &a::ADC, &m1_ARI},  {"ADC", &a::ADC, &m1_ARR},  {"ADC", &a::ADC, &m1_RRI},  {"ADC", &a::ADC, &m1_RRR},  {"ADC", &a::ADC, &m1_LLI},  {"UMLAL", &a::UMLAL, &XXX},  {"ADC", &a::ADC, &m1_LRI},  {"STRH", &a::STRH, &m3_RGP}, {"ADC", &a::ADC, &m1_ARI},  {"???", &a::NOP, &XXX},        {"ADC", &a::ADC, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"ADCS", &a::ADC, &m1_LLI}, {"ADCS", &a::ADC, &m1_LLR}, {"ADCS", &a::ADC, &m1_LRI}, {"ADCS", &a::ADC, &m1_LRR}, {"ADCS", &a::ADC, &m1_ARI}, {"ADCS", &a::ADC, &m1_ARR}, {"ADCS", &a::ADC, &m1_RRI}, {"ADCS", &a::ADC, &m1_RRR}, {"ADCS", &a::ADC, &m1_LLI}, {"UMLALS", &a::UMLAL, &XXX}, {"ADCS", &a::ADC, &m1_LRI}, {"LDRH", &a::LDRH, &m3_RGP}, {"ADCS", &a::ADC, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_RGP}, {"ADCS", &a::ADC, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_RGP}},
		{{"SBC", &a::SBC, &m1_LLI},  {"SBC", &a::SBC, &m1_LLR},  {"SBC", &a::SBC, &m1_LRI},  {"SBC", &a::SBC, &m1_LRR},  {"SBC", &a::SBC, &m1_ARI},  {"SBC", &a::SBC, &m1_ARR},  {"SBC", &a::SBC, &m1_RRI},  {"SBC", &a::SBC, &m1_RRR},  {"SBC", &a::SBC, &m1_LLI},  {"SMULL", &a::SMULL, &XXX},  {"SBC", &a::SBC, &m1_LRI},  {"STRH", &a::STRH, &m3_IMP}, {"SBC", &a::SBC, &m1_ARI},  {"???", &a::NOP, &XXX},        {"SBC", &a::SBC, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"SBCS", &a::SBC, &m1_LLI}, {"SBCS", &a::SBC, &m1_LLR}, {"SBCS", &a::SBC, &m1_LRI}, {"SBCS", &a::SBC, &m1_LRR}, {"SBCS", &a::SBC, &m1_ARI}, {"SBCS", &a::SBC, &m1_ARR}, {"SBCS", &a::SBC, &m1_RRI}, {"SBCS", &a::SBC, &m1_RRR}, {"SBCS", &a::SBC, &m1_LLI}, {"SMULLS", &a::SMULL, &XXX}, {"SBCS", &a::SBC, &m1_LRI}, {"LDRH", &a::LDRH, &m3_IMP}, {"SBCS", &a::SBC, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_IMP}, {"SBCS", &a::SBC, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_IMP}},
		{{"RSC", &a::RSC, &m1_LLI},  {"RSC", &a::RSC, &m1_LLR},  {"RSC", &a::RSC, &m1_LRI},  {"RSC", &a::RSC, &m1_LRR},  {"RSC", &a::RSC, &m1_ARI},  {"RSC", &a::RSC, &m1_ARR},  {"RSC", &a::RSC, &m1_RRI},  {"RSC", &a::RSC, &m1_RRR},  {"RSC", &a::RSC, &m1_LLI},  {"SMLAL", &a::SMULL, &XXX},  {"RSC", &a::RSC, &m1_LRI},  {"STRH", &a::STRH, &m3_IMP}, {"RSC", &a::RSC, &m1_ARI},  {"???", &a::NOP, &XXX},        {"RSC", &a::RSC, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"RSCS", &a::RSC, &m1_LLI}, {"RSCS", &a::RSC, &m1_LLR}, {"RSCS", &a::RSC, &m1_LRI}, {"RSCS", &a::RSC, &m1_LRR}, {"RSCS", &a::RSC, &m1_ARI}, {"RSCS", &a::RSC, &m1_ARR}, {"RSCS", &a::RSC, &m1_RRI}, {"RSCS", &a::RSC, &m1_RRR}, {"RSCS", &a::RSC, &m1_LLI}, {"SMLALS", &a::SMULL, &XXX}, {"RSCS", &a::RSC, &m1_LRI}, {"LDRH", &a::LDRH, &m3_IMP}, {"RSCS", &a::RSC, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_IMP}, {"RSCS", &a::RSC, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_IMP}},
		{{"MRS", &a::MRS, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},	  {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"SWP", &a::SWP, &XXX},      {"???", &a::NOP, &XXX},     {"STRH", &a::STRH, &m3_RGO}, {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},        {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX}},
		{{"TSTS", &a::TST, &m1_LLI}, {"TSTS", &a::TST, &m1_LLR}, {"TSTS", &a::TST, &m1_LRI}, {"TSTS", &a::TST, &m1_LRR}, {"TSTS", &a::TST, &m1_ARI}, {"TSTS", &a::TST, &m1_ARR}, {"TSTS", &a::TST, &m1_RRI}, {"TSTS", &a::TST, &m1_RRR}, {"TSTS", &a::TST, &m1_LLI}, {"???", &a::NOP, &XXX},      {"TSTS", &a::TST, &m1_LRI}, {"LDRH", &a::LDRH, &m3_RGO}, {"TSTS", &a::TST, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_RGO}, {"TSTS", &a::TST, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_RGO}},
		{{"MSR", &a::MSR, &XXX},     {"BX", &a::BX, &XXX},       {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},      {"???", &a::NOP, &XXX},     {"STRH", &a::STRH, &m3_PRG}, {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},        {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX}},
		{{"TEQS", &a::TEQ, &m1_LLI}, {"TEQS", &a::TEQ, &m1_LLR}, {"TEQS", &a::TEQ, &m1_LRI}, {"TEQS", &a::TEQ, &m1_LRR}, {"TEQS", &a::TEQ, &m1_ARI}, {"TEQS", &a::TEQ, &m1_ARR}, {"TEQS", &a::TEQ, &m1_RRI}, {"TEQS", &a::TEQ, &m1_RRR}, {"TEQS", &a::TEQ, &m1_LLI}, {"???", &a::NOP, &XXX},      {"TEQS", &a::TEQ, &m1_LRI}, {"LDRH", &a::LDRH, &m3_PRG}, {"TEQS", &a::TEQ, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_PRG}, {"TEQS", &a::TEQ, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_PRG}},
		{{"MRS", &a::MRS, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},	  {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"SWPB", &a::SWPB, &XXX},    {"???", &a::NOP, &XXX},     {"STRH", &a::STRH, &m3_IMO}, {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},        {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX}},
		{{"CMP", &a::CMP, &m1_LLI}, {"CMP", &a::CMP, &m1_LLR}, {"CMP", &a::CMP, &m1_LRI}, {"CMP", &a::CMP, &m1_LRR}, {"CMP", &a::CMP, &m1_ARI}, {"CMP", &a::CMP, &m1_ARR}, {"CMP", &a::CMP, &m1_RRI}, {"CMP", &a::CMP, &m1_RRR}, {"CMP", &a::CMP, &m1_LLI}, {"???", &a::NOP, &XXX},      {"CMP", &a::CMP, &m1_LRI}, {"LDRH", &a::LDRH, &m3_IMO}, {"CMP", &a::CMP, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_IMO}, {"CMP", &a::CMP, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_IMO}},
		{{"MSR", &a::MSR, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},      {"???", &a::NOP, &XXX},     {"STRH", &a::STRH, &m3_PIM}, {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},        {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX}},
		{{"CMN", &a::CMN, &m1_LLI}, {"CMN", &a::CMN, &m1_LLR}, {"CMN", &a::CMN, &m1_LRI}, {"CMN", &a::CMN, &m1_LRR}, {"CMN", &a::CMN, &m1_ARI}, {"CMN", &a::CMN, &m1_ARR}, {"CMN", &a::CMN, &m1_RRI}, {"CMN", &a::CMN, &m1_RRR}, {"CMN", &a::CMN, &m1_LLI}, {"???", &a::NOP, &XXX},      {"CMN", &a::CMN, &m1_LRI}, {"LDRH", &a::LDRH, &m3_PIM}, {"CMN", &a::CMN, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_PIM}, {"CMN", &a::CMN, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_PIM}},
		{{"ORR", &a::ORR, &m1_LLI},  {"ORR", &a::ORR, &m1_LLR},  {"ORR", &a::ORR, &m1_LRI},  {"ORR", &a::ORR, &m1_LRR},  {"ORR", &a::ORR, &m1_ARI},  {"ORR", &a::ORR, &m1_ARR},  {"ORR", &a::ORR, &m1_RRI},  {"ORR", &a::ORR, &m1_RRR},  {"ORR", &a::ORR, &m1_LLI},  {"???", &a::NOP, &XXX},      {"ORR", &a::ORR, &m1_LRI},  {"STRH", &a::STRH, &m3_RGO}, {"ORR", &a::ORR, &m1_ARI},  {"???", &a::NOP, &XXX},        {"ORR", &a::ORR, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"ORRS", &a::ORR, &m1_LLI}, {"ORRS", &a::ORR, &m1_LLR}, {"ORRS", &a::ORR, &m1_LRI}, {"ORRS", &a::ORR, &m1_LRR}, {"ORRS", &a::ORR, &m1_ARI}, {"ORRS", &a::ORR, &m1_ARR}, {"ORRS", &a::ORR, &m1_RRI}, {"ORRS", &a::ORR, &m1_RRR}, {"ORRS", &a::ORR, &m1_LLI}, {"???", &a::NOP, &XXX},      {"ORRS", &a::ORR, &m1_LRI}, {"LDRH", &a::LDRH, &m3_RGO}, {"ORRS", &a::ORR, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_RGO}, {"ORRS", &a::ORR, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_RGO}},
		{{"MOV", &a::MOV, &m1_LLI},  {"MOV", &a::MOV, &m1_LLR},  {"MOV", &a::MOV, &m1_LRI},  {"MOV", &a::MOV, &m1_LRR},  {"MOV", &a::MOV, &m1_ARI},  {"MOV", &a::MOV, &m1_ARR},  {"MOV", &a::MOV, &m1_RRI},  {"MOV", &a::MOV, &m1_RRR},  {"MOV", &a::MOV, &m1_LLI},  {"???", &a::NOP, &XXX},      {"MOV", &a::MOV, &m1_LRI},  {"STRH", &a::STRH, &m3_PRG}, {"MOV", &a::MOV, &m1_ARI},  {"???", &a::NOP, &XXX},        {"MOV", &a::MOV, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"MOVS", &a::MOV, &m1_LLI}, {"MOVS", &a::MOV, &m1_LLR}, {"MOVS", &a::MOV, &m1_LRI}, {"MOVS", &a::MOV, &m1_LRR}, {"MOVS", &a::MOV, &m1_ARI}, {"MOVS", &a::MOV, &m1_ARR}, {"MOVS", &a::MOV, &m1_RRI}, {"MOVS", &a::MOV, &m1_RRR}, {"MOVS", &a::MOV, &m1_LLI}, {"???", &a::NOP, &XXX},      {"MOVS", &a::MOV, &m1_LRI}, {"LDRH", &a::LDRH, &m3_PRG}, {"MOVS", &a::MOV, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_PRG}, {"MOVS", &a::MOV, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_PRG}},
		{{"BIC", &a::BIC, &m1_LLI},  {"BIC", &a::BIC, &m1_LLR},  {"BIC", &a::BIC, &m1_LRI},  {"BIC", &a::BIC, &m1_LRR},  {"BIC", &a::BIC, &m1_ARI},  {"BIC", &a::BIC, &m1_ARR},  {"BIC", &a::BIC, &m1_RRI},  {"BIC", &a::BIC, &m1_RRR},  {"BIC", &a::BIC, &m1_LLI},  {"???", &a::NOP, &XXX},      {"BIC", &a::BIC, &m1_LRI},  {"STRH", &a::STRH, &m3_IMO}, {"BIC", &a::BIC, &m1_ARI},  {"???", &a::NOP, &XXX},        {"BIC", &a::BIC, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"BICS", &a::BIC, &m1_LLI}, {"BICS", &a::BIC, &m1_LLR}, {"BICS", &a::BIC, &m1_LRI}, {"BICS", &a::BIC, &m1_LRR}, {"BICS", &a::BIC, &m1_ARI}, {"BICS", &a::BIC, &m1_ARR}, {"BICS", &a::BIC, &m1_RRI}, {"BICS", &a::BIC, &m1_RRR}, {"BICS", &a::BIC, &m1_LLI}, {"???", &a::NOP, &XXX},      {"BICS", &a::BIC, &m1_LRI}, {"LDRH", &a::LDRH, &m3_IMO}, {"BICS", &a::BIC, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_IMO}, {"BICS", &a::BIC, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_IMO}},
		{{"MVN", &a::MVN, &m1_LLI},  {"MVN", &a::MVN, &m1_LLR},  {"MVN", &a::MVN, &m1_LRI},  {"MVN", &a::MVN, &m1_LRR},  {"MVN", &a::MVN, &m1_ARI},  {"MVN", &a::MVN, &m1_ARR},  {"MVN", &a::MVN, &m1_RRI},  {"MVN", &a::MVN, &m1_RRR},  {"MVN", &a::MVN, &m1_LLI},  {"???", &a::NOP, &XXX},      {"MVN", &a::MVN, &m1_LRI},  {"STRH", &a::STRH, &m3_PIM}, {"MVN", &a::MVN, &m1_ARI},  {"???", &a::NOP, &XXX},        {"MVN", &a::MVN, &m1_RRI},  {"???", &a::NOP, &XXX}},
		{{"MVNS", &a::MVN, &m1_LLI}, {"MVNS", &a::MVN, &m1_LLR}, {"MVNS", &a::MVN, &m1_LRI}, {"MVNS", &a::MVN, &m1_LRR}, {"MVNS", &a::MVN, &m1_ARI}, {"MVNS", &a::MVN, &m1_ARR}, {"MVNS", &a::MVN, &m1_RRI}, {"MVNS", &a::MVN, &m1_RRR}, {"MVNS", &a::MVN, &m1_LLI}, {"???", &a::NOP, &XXX},      {"MVNS", &a::MVN, &m1_LRI}, {"LDRH", &a::LDRH, &m3_PIM}, {"MVNS", &a::MVN, &m1_ARI}, {"LDRSB", &a::LDRSB, &m3_PIM}, {"MVNS", &a::MVN, &m1_RRI}, {"LDRSH", &a::LDRSH, &m3_PIM}},
		
		{{"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},   {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},   {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM},     {"AND", &a::AND, &m1_IMM},  {"AND", &a::AND, &m1_IMM}},
		{{"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM},  {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM},  {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM},    {"ANDS", &a::AND, &m1_IMM}, {"ANDS", &a::AND, &m1_IMM}},
		{{"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},   {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},   {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM},     {"EOR", &a::EOR, &m1_IMM},  {"EOR", &a::EOR, &m1_IMM}},
		{{"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM},  {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM},  {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM},    {"EORS", &a::EOR, &m1_IMM}, {"EORS", &a::EOR, &m1_IMM}},
		{{"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},	{"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},   {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM},     {"SUB", &a::SUB, &m1_IMM},  {"SUB", &a::SUB, &m1_IMM}},
		{{"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM},  {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM},  {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM},    {"SUBS", &a::SUB, &m1_IMM}, {"SUBS", &a::SUB, &m1_IMM}},
		{{"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},	{"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},   {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM},     {"RSB", &a::RSB, &m1_IMM},  {"RSB", &a::RSB, &m1_IMM}},
		{{"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM},  {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM},  {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM},    {"RSBS", &a::RSB, &m1_IMM}, {"RSBS", &a::RSB, &m1_IMM}},
		{{"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},	{"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},   {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM},     {"ADD", &a::ADD, &m1_IMM},  {"ADD", &a::ADD, &m1_IMM}},
		{{"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM},  {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM},  {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM},    {"ADDS", &a::ADD, &m1_IMM}, {"ADDS", &a::ADD, &m1_IMM}},
		{{"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},	{"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},   {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM},     {"ADC", &a::ADC, &m1_IMM},  {"ADC", &a::ADC, &m1_IMM}},
		{{"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM},  {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM},  {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM},    {"ADCS", &a::ADC, &m1_IMM}, {"ADCS", &a::ADC, &m1_IMM}},
		{{"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},	{"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},   {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM},     {"SBC", &a::SBC, &m1_IMM},  {"SBC", &a::SBC, &m1_IMM}},
		{{"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM},  {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM},  {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM},    {"SBCS", &a::SBC, &m1_IMM}, {"SBCS", &a::SBC, &m1_IMM}},
		{{"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},	{"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},   {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM},     {"RSC", &a::RSC, &m1_IMM},  {"RSC", &a::RSC, &m1_IMM}},
		{{"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM},  {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM},  {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM},    {"RSCS", &a::RSC, &m1_IMM}, {"RSCS", &a::RSC, &m1_IMM}},
		{{"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},      {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},      {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},        {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX}},
		{{"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM},  {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM},  {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM},    {"TSTS", &a::TST, &m1_IMM}, {"TSTS", &a::TST, &m1_IMM}},
		{{"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},   {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},   {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},     {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM}},
		{{"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM},  {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM},  {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM},    {"TEQS", &a::TEQ, &m1_IMM}, {"TEQS", &a::TEQ, &m1_IMM}},
		{{"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},      {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},      {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX},        {"???", &a::NOP, &XXX},     {"???", &a::NOP, &XXX}},
		{{"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM},  {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM},  {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM},    {"CMP", &a::CMP, &m1_IMM}, {"CMP", &a::CMP, &m1_IMM}},
		{{"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},   {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},   {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM},     {"MSR", &a::MSR, &m1_IMM},  {"MSR", &a::MSR, &m1_IMM}},
		{{"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM},  {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM},  {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM},    {"CMN", &a::CMN, &m1_IMM}, {"CMN", &a::CMN, &m1_IMM}},
		{{"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},   {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},   {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM},     {"ORR", &a::ORR, &m1_IMM},  {"ORR", &a::ORR, &m1_IMM}},
		{{"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM},  {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM},  {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM},    {"ORRS", &a::ORR, &m1_IMM}, {"ORRS", &a::ORR, &m1_IMM}},
		{{"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},   {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},   {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM},     {"MOV", &a::MOV, &m1_IMM},  {"MOV", &a::MOV, &m1_IMM}},
		{{"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM},  {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM},  {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM},    {"MOVS", &a::MOV, &m1_IMM}, {"MOVS", &a::MOV, &m1_IMM}},
		{{"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},   {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},   {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM},     {"BIC", &a::BIC, &m1_IMM},  {"BIC", &a::BIC, &m1_IMM}},
		{{"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM},  {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM},  {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM},    {"BICS", &a::BIC, &m1_IMM}, {"BICS", &a::BIC, &m1_IMM}},
		{{"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},   {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},   {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM},     {"MVN", &a::MVN, &m1_IMM},  {"MVN", &a::MVN, &m1_IMM}},
		{{"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM},  {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM},  {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM},    {"MVNS", &a::MVN, &m1_IMM}, {"MVNS", &a::MVN, &m1_IMM}},
		{{"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},   {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},   {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},     {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP}},
		{{"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},   {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},   {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},     {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP}},
		{{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP}, {"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP}, {"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},   {"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP}},
		{{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP}, {"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP}, {"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},   {"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP}},
		{{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP}, {"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP}, {"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},   {"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP}},
		{{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP}, {"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP}, {"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},   {"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP}},
		{{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP}, {"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP}, {"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},   {"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP}},
		{{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP}, {"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP}, {"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},   {"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP}},
		{{"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},   {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},   {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP},     {"STR", &a::STR, &m2_IMP},  {"STR", &a::STR, &m2_IMP}},
		{{"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},   {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},   {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP},     {"LDR", &a::LDR, &m2_IMP},  {"LDR", &a::LDR, &m2_IMP}},
		{{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP}, {"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP}, {"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP},   {"STRT", &a::STRT, &m2_IMP},{"STRT", &a::STRT, &m2_IMP}},
		{{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP}, {"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP}, {"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP},   {"LDRT", &a::LDRT, &m2_IMP},{"LDRT", &a::LDRT, &m2_IMP}},
		{{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP}, {"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP}, {"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP},   {"STRB", &a::STRB, &m2_IMP},{"STRB", &a::STRB, &m2_IMP}},
		{{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP}, {"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP}, {"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP},   {"LDRB", &a::LDRB, &m2_IMP},{"LDRB", &a::LDRB, &m2_IMP}},
		{{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP}, {"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP}, {"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP},   {"STRBT",&a::STRBT,&m2_IMP},{"STRBT",&a::STRBT,&m2_IMP}},
		{{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP}, {"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP}, {"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP},   {"LDRBT",&a::LDRBT,&m2_IMP},{"LDRBT",&a::LDRBT,&m2_IMP}},
		{{"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},   {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},   {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},     {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO}},
		{{"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},   {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},   {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},     {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO}},
		{{"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},   {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},   {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},     {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM}},
		{{"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},   {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},   {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},     {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM}},
		{{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO}, {"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO}, {"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},   {"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO}},
		{{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO}, {"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO}, {"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},   {"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO}},
		{{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM}, {"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM}, {"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},   {"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM}},
		{{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM}, {"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM}, {"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},   {"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM}},
		{{"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},   {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},   {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO},     {"STR", &a::STR, &m2_IMO},  {"STR", &a::STR, &m2_IMO}},
		{{"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},   {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},   {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO},     {"LDR", &a::LDR, &m2_IMO},  {"LDR", &a::LDR, &m2_IMO}},
		{{"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},   {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},   {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM},     {"STR", &a::STR, &m2_PIM},  {"STR", &a::STR, &m2_PIM}},
		{{"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},   {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},   {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM},     {"LDR", &a::LDR, &m2_PIM},  {"LDR", &a::LDR, &m2_PIM}},
		{{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO}, {"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO}, {"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO},   {"STRB", &a::STRB, &m2_IMO},{"STRB", &a::STRB, &m2_IMO}},
		{{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO}, {"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO}, {"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO},   {"LDRB", &a::LDRB, &m2_IMO},{"LDRB", &a::LDRB, &m2_IMO}},
		{{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM}, {"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM}, {"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM},   {"STRB", &a::STRB, &m2_PIM},{"STRB", &a::STRB, &m2_PIM}},
		{{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM}, {"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM}, {"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM},   {"LDRB", &a::LDRB, &m2_PIM},{"LDRB", &a::LDRB, &m2_PIM}},
		//60
		{{"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},   {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},    {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX}},
		{{"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},   {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},    {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX}},
		{{"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STRT", &a::STRT, &m2_SRP},{"???", &a::NOP, &XXX},     {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},   {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},    {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},      {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},      {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX}},
		{{"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDRT", &a::LDRT, &m2_SRP},{"???", &a::NOP, &XXX},     {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},   {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},    {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},      {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},      {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX}},
		{{"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRP},{"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},   {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},    {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX}},
		{{"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRP},{"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},   {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},    {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX}},
		{{"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},     {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},     {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},   {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},   {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},    {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},      {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},      {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX}},
		{{"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},     {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},     {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},   {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},   {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},    {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},      {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},      {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX}},
		{{"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},   {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},    {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_SRP},    {"???", &a::NOP, &XXX}},
		{{"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},   {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},    {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_SRP},    {"???", &a::NOP, &XXX}},
		{{"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STRT", &a::STRT, &m2_SRP},{"???", &a::NOP, &XXX},     {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},   {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},    {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},      {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX},      {"STRT", &a::STRT, &m2_SRP},  {"???", &a::NOP, &XXX}},
		{{"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDRT", &a::LDRT, &m2_SRP},{"???", &a::NOP, &XXX},     {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},   {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},    {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},      {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX},      {"LDRT", &a::LDRT, &m2_SRP},  {"???", &a::NOP, &XXX}},
		{{"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRP},{"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},   {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},    {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_SRP},  {"???", &a::NOP, &XXX}},
		{{"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRP},{"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},   {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},    {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_SRP},  {"???", &a::NOP, &XXX}},
		{{"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},     {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},     {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},   {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},   {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},    {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},      {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX},      {"STRBT", &a::STRBT, &m2_SRP},{"???", &a::NOP, &XXX}},
		{{"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},     {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},     {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},   {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},   {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},    {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},      {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX},      {"LDRBT", &a::LDRBT, &m2_SRP},{"???", &a::NOP, &XXX}},
		{{"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRO},  {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},   {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},    {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX}},
		{{"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRO},  {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},   {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},    {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX}},
		{{"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_PSR},  {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},   {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},    {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX}},
		{{"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_PSR},  {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},   {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},    {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX}},
		{{"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRO},{"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},   {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},    {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX}},
		{{"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRO},{"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},   {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},    {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX}},
		{{"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_PSR},{"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},   {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},    {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX}},
		{{"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_PSR},{"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},   {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},    {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX}},
		{{"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRO},  {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},   {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},    {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_SRO},    {"???", &a::NOP, &XXX}},
		{{"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRO},  {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},   {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},    {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_SRO},    {"???", &a::NOP, &XXX}},
		{{"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_PSR},  {"???", &a::NOP, &XXX},     {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},   {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},    {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX},      {"STR", &a::STR, &m2_PSR},    {"???", &a::NOP, &XXX}},
		{{"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_PSR},  {"???", &a::NOP, &XXX},     {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},   {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},    {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX},      {"LDR", &a::LDR, &m2_PSR},    {"???", &a::NOP, &XXX}},
		{{"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRO},{"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},   {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},    {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_SRO},  {"???", &a::NOP, &XXX}},
		{{"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRO},{"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},   {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},    {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_SRO},  {"???", &a::NOP, &XXX}},
		{{"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_PSR},{"???", &a::NOP, &XXX},     {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},   {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},    {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX},      {"STRB", &a::STRB, &m2_PSR},  {"???", &a::NOP, &XXX}},
		{{"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_PSR},{"???", &a::NOP, &XXX},     {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},   {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},    {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX},      {"LDRB", &a::LDRB, &m2_PSR},  {"???", &a::NOP, &XXX}},
		//80
		{{"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}},
		{{"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}},
		{{"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}, {"STMDA", &a::STM_1, &m4_DA}},
		{{"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}, {"LDMDA", &a::LDM_1, &m4_DA}},
		{{"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}},
		{{"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}},
		{{"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}, {"STMDA", &a::STM_2, &m4_DA}},
		{{"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}, {"LDMDA", &a::LDM_2, &m4_DA}},
		{{"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}},
		{{"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}},
		{{"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}, {"STMIA", &a::STM_1, &m4_IA}},
		{{"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}, {"LDMIA", &a::LDM_1, &m4_IA}},
		{{"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}},
		{{"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}},
		{{"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}, {"STMIA", &a::STM_2, &m4_IA}},
		{{"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}, {"LDMIA", &a::LDM_2, &m4_IA}},
		{{"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}},
		{{"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}},
		{{"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}, {"STMDB", &a::STM_1, &m4_DB}},
		{{"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}, {"LDMDB", &a::LDM_1, &m4_DB}},
		{{"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}},
		{{"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}},
		{{"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}, {"STMDB", &a::STM_2, &m4_DB}},
		{{"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}, {"LDMDB", &a::LDM_2, &m4_DB}},
		{{"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}},
		{{"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}},
		{{"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}, {"STMIB", &a::STM_1, &m4_IB}},
		{{"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}, {"LDMIB", &a::LDM_1, &m4_IB}},
		{{"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}},
		{{"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}},
		{{"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}, {"STMIB", &a::STM_2, &m4_IB}},
		{{"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}, {"LDMIB", &a::LDM_2, &m4_IB}},
		//A0
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		{{"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}, {"B", &a::B, &XXX}},
		//B0
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		{{"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}, {"BL", &a::B, &XXX}},
		//BF
		//Add Multiprocessor Instructions later if needed
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		{{"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}, {"???", &a::NOP, &XXX}},
		//F0
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
		{{"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}, {"SWI", &a::SWI, &XXX}},
	};
	
}

ARM7TDMI::~ARM7TDMI() {

}

void ARM7TDMI::reset()
{
	prefetch = 0;
}
// assuming 32bit instructions
bool ARM7TDMI::clock(bool instantly)
{
	if (cycles == 0) {
		pc = prefetch;
		opcode = read(pc);
		cycles += (this->*(getInstruction(opcode).addrmode->addrmode_f))();
		cycles += (this->*(getInstruction(opcode).operate))();
		prefetch += sizeof(uint32_t); // fix?
		if (instantly) cycles = 0;
		return true;
	}
	--cycles;
	return false;
}

//bus comm
uint32_t ARM7TDMI::read(uint32_t a, uint32_t size)
{
	if (size == 32) {
		return bus->cpuRead(a, false);
	}
	else if (size == 16) {
		return (bus->cpuRead(a, false) >> ((a%2)*16)) & 0xffff;
	}
	else if (size == 8) {
		return (bus->cpuRead(a, false) >> ((a%4)*8)) & 0xff;
	}
	else {
		//error - add exception handler later
		return 0;
	}
}

void ARM7TDMI::write(uint32_t a, uint32_t data)
{
	write(a, data, 32);
}
void ARM7TDMI::write(uint32_t a, uint32_t data, uint32_t size)
{
	if (size == 32) {
		bus->cpuWrite(a, false);
	}
	else if (size == 16) {
		uint32_t d = bus->cpuRead(a, true);
		d &= (0xffff << (1 - (a % 2)) * 16);
		d |= (data & 0xffff) << ((a % 2) * 16);
		bus->cpuWrite(a, d);
	}
	else if (size == 8) {
		uint32_t d = bus->cpuRead(a, true);
		d &= rotateRight(0xffffff, (3 - (a % 4)) * 8);
		d |= (data & 0xff) << ((a % 4) * 8);
		bus->cpuWrite(a, d);
	}
	else {
		//error - add exception handler later
	}
}
void ARM7TDMI::write(uint16_t a, uint16_t data)
{
}

//register helpers
uint32_t ARM7TDMI::readRegister(uint32_t n, uint8_t force_mode) {
	if (n <= 7) {
		return r[n];
	}
	else if (n <= 12) {
		if (force_mode == 6) return r_fiq[n - 8];
		else return r[n];
	}
	else if (n == 13) {
		if (force_mode < 2) return sp;
		else return sp_banked[force_mode - 2];
	}
	else if (n == 14) {
		if (force_mode < 2) return lr;
		else return lr_banked[force_mode - 2];
	}
	else if (n == 15) {
		return pc + 8;
	}
	else throw std::invalid_argument("Wrong input to readRegister: " + std::to_string(n));
}
uint32_t ARM7TDMI::readRegister(uint32_t n) {
	return readRegister(n, mode);
}

void ARM7TDMI::writeRegister(uint32_t n, uint32_t data, uint8_t force_mode) {
	if (n <= 7) {
		r[n] = data;
	}
	else if (n <= 12) {
		if (force_mode == 6) r_fiq[n - 8] = data;
		else r[n] = data;
	}
	else if (n == 13) {
		if (force_mode < 2) sp = data;
		else sp_banked[force_mode - 2] = data;
	}
	else if (n == 14) {
		if (force_mode < 2) lr = data;
		else lr_banked[force_mode - 2] = data;
	}
	else if (n == 15) {
		pc = data;
	}
}
void ARM7TDMI::writeRegister(uint32_t n, uint32_t data) {
	writeRegister(n, data, mode);
}

ARM7TDMI::ProgStatReg ARM7TDMI::getSPSR() {
	if (mode > 1) {
		return spsr[mode - 2];
	}
	else {
		//unpredictable
		return spsr[0]; //lol, I don't know. It should be unpredictable.
	}
}

void ARM7TDMI::setSPSR(uint32_t data) {
	if (mode > 1) {
		spsr[mode - 2].reg = data;
	}
	else {
		//unpredictable
		spsr[0].reg = data; //lol, I don't know. It should be unpredictable.
	}
}

std::string ARM7TDMI::getRegisterName(uint32_t num) {
	if (num == 10) return "SL";
	if (num == 11) return "FP";
	if (num == 12) return "IP";
	if (num == 13) return "SP";
	if (num == 14) return "LR";
	if (num == 15) return "PC";
	return "R" + std::to_string(num);
}

//condition code
bool ARM7TDMI::conditionPassed() {
	return (this->*(condition_lookup[(opcode >> 28) & 0b1111].cond))();
}

bool ARM7TDMI::EQ() {
	return cpsr.Z == 1;
}

bool ARM7TDMI::NE() {
	return cpsr.Z == 0;
}

bool ARM7TDMI::CS() {
	return cpsr.C == 1;
}

bool ARM7TDMI::CC() {
	return cpsr.C == 0;
}

bool ARM7TDMI::MI() {
	return cpsr.N == 1;
}

bool ARM7TDMI::PL() {
	return cpsr.N == 0;
}

bool ARM7TDMI::VS() {
	return cpsr.V == 1;
}

bool ARM7TDMI::VC() {
	return cpsr.V == 0;
}

bool ARM7TDMI::HI() {
	return cpsr.C == 1 && cpsr.Z == 0;
}

bool ARM7TDMI::LS() {
	return cpsr.C == 0 || cpsr.Z == 1;
}

bool ARM7TDMI::GE() {
	return cpsr.N == cpsr.V;
}

bool ARM7TDMI::LT() {
	return cpsr.N != cpsr.V;
}

bool ARM7TDMI::GT() {
	return cpsr.Z == 0 && (cpsr.N == cpsr.V);
}

bool ARM7TDMI::LE() {
	return cpsr.Z == 1 || (cpsr.N != cpsr.V);
}

bool ARM7TDMI::AL() {
	return true;
}

bool ARM7TDMI::NV() {
	return false;
}

//addressing modes
//XXX
uint32_t ARM7TDMI::XXX_f() {
	return 0;
}

//Mode 1
uint32_t ARM7TDMI::m1_IMM_f() {
	uint32_t rotate_imm = (opcode >> 8) & 0xf;
	uint32_t immed_8 = (opcode & 0xff);
	shifter_operand = rotateRight(immed_8, rotate_imm * 2);
	if (rotate_imm == 0) {
		shifter_carry_out = cpsr.C;
	}
	else {
		shifter_carry_out = shifter_operand >> 31;
	}
	return 0;
}

uint32_t ARM7TDMI::m1_REG_f() {
	shifter_operand = readRegister(opcode & 0b1111);
	shifter_carry_out = cpsr.C;
	return 0;
}

uint32_t ARM7TDMI::m1_LLI_f() {
	uint32_t shift_imm = (opcode >> 7) & 0b11111;
	uint32_t Rm = readRegister(opcode & 0b1111);
	if (shift_imm == 0) {
		shifter_operand = Rm;
		shifter_carry_out = cpsr.C;
	}
	else {
		shifter_operand = Rm << shift_imm;
		shifter_carry_out = (Rm >> (32 - shift_imm)) & 0x1;
	}
	return 0;
}

uint32_t ARM7TDMI::m1_LLR_f() {
	//uint32_t Rs = readRegister((opcode >> 8) & 0xf);
	uint32_t Rs_LSB = readRegister((opcode >> 8) & 0xf) & 0xff; //least significant byte of Rs
	uint32_t Rm = readRegister(opcode & 0xf);

	if (Rs_LSB == 0) {
		shifter_operand = Rm;
		shifter_carry_out = cpsr.C;
	}
	else if (Rs_LSB < 32) {
		shifter_operand = Rm << Rs_LSB;
		shifter_carry_out = (Rm >> (32 - Rs_LSB)) & 0x1;
	}
	else if (Rs_LSB == 32) {
		shifter_operand = 0;
		shifter_carry_out = Rm & 0x1;
	}
	else {
		shifter_operand = 0;
		shifter_carry_out = 0;
	}
	return 1; //additional cycle
}

uint32_t ARM7TDMI::m1_LRI_f() {
	uint32_t Rm = readRegister(opcode & 0xf);
	uint32_t shift_imm = (opcode >> 7) & 0b11111;
	if (shift_imm == 0) {
		shifter_operand = 0;
		shifter_carry_out = Rm >> 31;
	}
	else {
		shifter_operand = Rm >> shift_imm;
		shifter_carry_out = (Rm >> (shift_imm - 1)) & 0x1;
	}
	return 0;
}

uint32_t ARM7TDMI::m1_LRR_f() {
	//uint32_t Rs = readRegister((opcode >> 8) & 0xf);
	uint32_t Rs_LSB = readRegister((opcode >> 8) & 0xf) & 0xff; //least significant byte of Rs
	uint32_t Rm = readRegister(opcode & 0xf);

	if (Rs_LSB == 0) {
		shifter_operand = Rm;
		shifter_carry_out = cpsr.C;
	}
	else if (Rs_LSB < 32) {
		shifter_operand = Rm >> Rs_LSB;
		shifter_carry_out = (Rm >> (Rs_LSB - 1)) & 0x1;
	}
	else if (Rs_LSB == 32) {
		shifter_operand = 0;
		shifter_carry_out = Rm >> 31;
	}
	else {
		shifter_operand = 0;
		shifter_carry_out = 0;
	}
	return 1;
}

uint32_t ARM7TDMI::m1_ARI_f() {
	uint32_t shift_imm = (opcode >> 7) & 0b11111;
	uint32_t Rm = readRegister(opcode & 0xf);
	if (shift_imm == 0) {
		if (Rm >> 31 == 0) {
			shifter_operand = 0;
		}
		else {
			shifter_operand = 0xffffffff;
		}
		shifter_carry_out = Rm >> 31;
	}
	else {
		//shifter_operand = (Rm >> 31 == 0) ? (Rm >> shift_imm) 
										  //: (Rm >> shift_imm) | ~(~(uint32_t)0 >> shift_imm); //arithmetic shift
		shifter_operand = arithmeticShift(Rm, shift_imm);
		shifter_carry_out = (Rm >> (shift_imm - 1)) & 0x1;
	}
	return 0;
}

uint32_t ARM7TDMI::m1_ARR_f() {
	uint32_t Rs_LSB = readRegister((opcode >> 8) & 0xf) & 0xff; //least significant byte of Rs
	uint32_t Rm = readRegister(opcode & 0xf);
	if (Rs_LSB == 0) {
		shifter_operand = Rm;
		shifter_carry_out = cpsr.C;
	}
	else if (Rs_LSB < 32) {
		//shifter_operand = (Rm >> 31 == 0) ? (Rm >> Rs_LSB)
		//								  : (Rm >> Rs_LSB) | ~(~(uint32_t)0 >> Rs_LSB); //arithmetic shift
		shifter_operand = arithmeticShift(Rm, Rs_LSB);
		shifter_carry_out = (Rm >> (Rs_LSB - 1)) & 0x1;
	}
	else {
		if (Rm >> 31 == 0) {
			shifter_operand = 0;
		}
		else {
			shifter_operand = 0xffffffff;
		}
		shifter_carry_out = Rm >> 31;
	}
	return 1;
}

uint32_t ARM7TDMI::m1_RRI_f() {
	uint32_t shift_imm = (opcode >> 7) & 0b11111;
	uint32_t Rm = readRegister(opcode & 0xf);
	if (shift_imm == 0) {
		//this is RRX. Should I write return m1_RRX() ?
	}
	else {
		shifter_operand = rotateRight(Rm, shift_imm);
		shifter_carry_out = Rm >> (shift_imm - 1);
	}
	return 0;
}

uint32_t ARM7TDMI::m1_RRR_f() {
	uint32_t Rs_LSB = readRegister((opcode >> 8) & 0xf) & 0xff; //least significant byte of Rs
	uint32_t Rs_4_0 = Rs_LSB & 0b11111;
	uint32_t Rm = readRegister(opcode & 0xf);
	if (Rs_LSB == 0) {
		shifter_operand = Rm;
		shifter_carry_out = cpsr.C;
	}
	else if (Rs_4_0 == 0) {
		shifter_operand = Rm;
		shifter_carry_out = Rm >> 31;
	}
	else {
		shifter_operand = rotateRight(Rm, Rs_4_0);
		shifter_carry_out = Rm >> (Rs_4_0 - 1);
	}
	return 0;
}

uint32_t ARM7TDMI::m1_RRX_f() {
	uint32_t Rm = readRegister(opcode & 0xf);
	shifter_operand = (Rm >> 1) | (cpsr.C << 31);
	shifter_carry_out = Rm & 0x1;
	return 0;
}

//Mode 2
uint32_t ARM7TDMI::m2_IMO_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t offset_12 = opcode & 0xfff;
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + offset_12;
	}
	else {
		ls_addr = Rn - offset_12;
	}
	return 0;
}
uint32_t ARM7TDMI::m2_RGO_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + Rm;
	}
	else {
		ls_addr = Rn - Rm;
	}
	return 0;
}
uint32_t ARM7TDMI::m2_SRO_f() {
	uint32_t shift = (opcode >> 5) & 0b11;
	uint32_t shift_imm = (opcode >> 7) & 0b11111;
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	uint32_t index = 0;
	switch (shift)
	{
		case 0b00: //LSL
			index = Rm << shift_imm;
			break;
		case 0b01: //LSR
			if (shift_imm == 0) index = 0;
			else index = Rm >> shift_imm;
			break;
		case 0b10: //ASR
			if (shift_imm == 0) index = 0xffffffff;
			else index = 0;
			break;
		case 0b11:
			if (shift_imm == 0)
				index = (cpsr.C << 31) | (Rm >> 1);
			else
				index = rotateRight(Rm, shift_imm);
			break;
	}
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + index;
	}
	else {
		ls_addr = Rn - index;
	}
	return 0;
}
uint32_t ARM7TDMI::m2_PIM_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t offset_12 = opcode & 0xfff;
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + offset_12;
	}
	else {
		ls_addr = Rn - offset_12;
	}
	if (conditionPassed()) {
		writeRegister((opcode >> 16) & 0xf, ls_addr); //write back into Rn
	}
	return 0;
}
uint32_t ARM7TDMI::m2_PRG_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + Rm;
	}
	else {
		ls_addr = Rn - Rm;
	}
	if (conditionPassed()) {
		writeRegister((opcode >> 16) & 0xf, ls_addr); //write back into Rn
	}
	return 0;
}
uint32_t ARM7TDMI::m2_PSR_f() {
	uint32_t shift = (opcode >> 5) & 0b11;
	uint32_t shift_imm = (opcode >> 7) & 0b11111;
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	uint32_t index = 0;
	switch (shift)
	{
	case 0b00: //LSL
		index = Rm << shift_imm;
		break;
	case 0b01: //LSR
		if (shift_imm == 0) index = 0;
		else index = Rm >> shift_imm;
		break;
	case 0b10: //ASR
		if (shift_imm == 0) index = 0xffffffff;
		else index = 0;
		break;
	case 0b11:
		if (shift_imm == 0)
			index = (cpsr.C << 31) | (Rm >> 1);
		else
			index = rotateRight(Rm, shift_imm);
		break;
	}
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + index;
	}
	else {
		ls_addr = Rn - index;
	}
	if (conditionPassed()) {
		writeRegister((opcode >> 16) & 0xf, ls_addr); //write back into Rn
	}
	return 0;
}
uint32_t ARM7TDMI::m2_IMP_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	ls_addr = Rn;
	if (conditionPassed()) {
		uint32_t offset_12 = opcode & 0xfff;
		if (((opcode >> 23) & 0x1) == 1) {
			writeRegister((opcode >> 16) & 0xf, Rn + offset_12);
		}
		else {
			writeRegister((opcode >> 16) & 0xf, Rn - offset_12);
		}
	}
	return 0;
}
uint32_t ARM7TDMI::m2_RGP_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	ls_addr = Rn;
	if (conditionPassed()) {
		uint32_t Rm = readRegister(opcode & 0xf);
		if (((opcode >> 23) & 0x1) == 1) {
			writeRegister((opcode >> 16) & 0xf, Rn + Rm);
		}
		else {
			writeRegister((opcode >> 16) & 0xf, Rn - Rm);
		}
	}
	return 0;
}
uint32_t ARM7TDMI::m2_SRP_f() {
	uint32_t shift = (opcode >> 5) & 0b11;
	uint32_t shift_imm = (opcode >> 7) & 0b11111;
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	uint32_t index = 0;
	ls_addr = Rn;
	switch (shift)
	{
	case 0b00: //LSL
		index = Rm << shift_imm;
		break;
	case 0b01: //LSR
		if (shift_imm == 0) index = 0;
		else index = Rm >> shift_imm;
		break;
	case 0b10: //ASR
		if (shift_imm == 0) index = 0xffffffff;
		else index = 0;
		break;
	case 0b11:
		if (shift_imm == 0)
			index = (cpsr.C << 31) | (Rm >> 1);
		else
			index = rotateRight(Rm, shift_imm);
		break;
	}
	
	if (conditionPassed()) {
		if (((opcode >> 23) & 0x1) == 1) {
			writeRegister((opcode >> 16) & 0xf, Rn + index);
		}
		else {
			writeRegister((opcode >> 16) & 0xf, Rn - index);
		}
	}
	return 0;
}

//Mode 3
uint32_t ARM7TDMI::m3_IMO_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t offset_8 = ((opcode >> 4) & 0xf0) | (opcode & 0xf);
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + offset_8;
	}
	else {
		ls_addr = Rn - offset_8;
	}
	return 0;
}
uint32_t ARM7TDMI::m3_RGO_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + Rm;
	}
	else {
		ls_addr = Rn - Rm;
	}
	return 0;
}
uint32_t ARM7TDMI::m3_PIM_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t offset_8 = ((opcode >> 4) & 0xf0) | (opcode & 0xf);
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + offset_8;
	}
	else {
		ls_addr = Rn - offset_8;
	}
	if (conditionPassed()) {
		writeRegister((opcode >> 16) & 0xf, ls_addr);
	}
	return 0;
}
uint32_t ARM7TDMI::m3_PRG_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	if (((opcode >> 23) & 0x1) == 1) {
		ls_addr = Rn + Rm;
	}
	else {
		ls_addr = Rn - Rm;
	}
	if (conditionPassed()) {
		writeRegister((opcode >> 16) & 0xf, ls_addr);
	}
	return 0;
}
uint32_t ARM7TDMI::m3_IMP_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	ls_addr = Rn;
	uint32_t offset_8 = ((opcode >> 4) & 0xf0) | (opcode & 0xf);
	
	if (conditionPassed()) {
		if (((opcode >> 23) & 0x1) == 1) {
			ls_addr = Rn + offset_8;
		}
		else {
			ls_addr = Rn - offset_8;
		}
	}
	return 0;
}
uint32_t ARM7TDMI::m3_RGP_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	ls_addr = Rn;
	if (conditionPassed()) {
		if (((opcode >> 23) & 0x1) == 1) {
			ls_addr = Rn + Rm;
		}
		else {
			ls_addr = Rn - Rm;
		}
	}
	return 0;
}

//Mode 4
uint32_t ARM7TDMI::m4_IA_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t num_set_bits = std::bitset<16>(opcode & 0xffff).count();
	ls_start_addr = Rn;
	ls_end_addr = Rn + (num_set_bits * 4) - 4;
	if (conditionPassed() && ((opcode >> 21) & 0x1) == 1) {
		writeRegister((opcode >> 16) & 0xf, Rn + num_set_bits * 4);
	}
	return 0;
}
uint32_t ARM7TDMI::m4_IB_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t num_set_bits = std::bitset<16>(opcode & 0xffff).count();
	ls_start_addr = Rn + 4;
	ls_end_addr = Rn + (num_set_bits * 4);
	if (conditionPassed() && ((opcode >> 21) & 0x1) == 1) {
		writeRegister((opcode >> 16) & 0xf, Rn + num_set_bits * 4);
	}
	return 0;
}
uint32_t ARM7TDMI::m4_DA_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t num_set_bits = std::bitset<16>(opcode & 0xffff).count();
	ls_start_addr = Rn - (num_set_bits * 4) + 4;
	ls_end_addr = Rn;
	if (conditionPassed() && ((opcode >> 21) & 0x1) == 1) {
		writeRegister((opcode >> 16) & 0xf, Rn - num_set_bits * 4);
	}
	return 0;
}
uint32_t ARM7TDMI::m4_DB_f() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t num_set_bits = std::bitset<16>(opcode & 0xffff).count();
	ls_start_addr = Rn - (num_set_bits * 4);
	ls_end_addr = Rn - 4;
	if (conditionPassed() && ((opcode >> 21) & 0x1) == 1) {
		writeRegister((opcode >> 16) & 0xf, Rn - num_set_bits * 4);
	}
	return 0;
}

//Instructions
//Move
uint32_t ARM7TDMI::MOV() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rd_addr = getBits(opcode, 12, 4);
		writeRegister(Rd_addr, shifter_operand);
		uint32_t Rd = readRegister(Rd_addr);
		
		if (getBit(opcode, 20) == 1) {
			if (Rd_addr == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = (Rd == 0);
				cpsr.C = shifter_carry_out;
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::MVN() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rd_addr = (opcode >> 12) & 0xf;
		writeRegister(Rd_addr, ~shifter_operand);
		uint32_t Rd = readRegister(Rd_addr);
		if (((opcode >> 20) & 0x1) == 1) {
			if (Rd_addr == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = (Rd == 0);
				cpsr.C = shifter_carry_out;
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::MRS() {
	if (conditionPassed()) {
		if ((opcode >> 22) & 0x1) {
			writeRegister((opcode >> 12) & 0xf, getSPSR().reg);
		}
		else {
			writeRegister((opcode >> 12) & 0xf, cpsr.reg);
		}
	}
	return 1;
}
uint32_t ARM7TDMI::MSR() {
	if (conditionPassed()) {
		uint32_t operand = 0;
		
		if ((opcode >> 25) & 0x1) {
			uint32_t immed_8 = (opcode & 0xff);
			uint32_t rotate_imm = (opcode >> 8) & 0xf;
			operand = rotateRight(immed_8, rotate_imm * 2);
		}
		else {
			operand = readRegister(opcode & 0xf);
		}
		if (((opcode >> 22) & 0x1) == 0) {
			if (((opcode >> 16) & 0x1) && mode > 0) {
				cpsr.reg &= 0xffffff00;
				cpsr.reg |= (operand & 0xff);
			} else if (((opcode >> 17) & 0x1) && mode > 0) {
				cpsr.reg &= 0xffff00ff;
				cpsr.reg |= ((operand >> 8) & 0xff) << 8;
			}
			else if (((opcode >> 18) & 0x1) && mode > 0) {
				cpsr.reg &= 0xff00ffff;
				cpsr.reg |= ((operand >> 16) & 0xff) << 16;
			}
			else if (((opcode >> 19) & 0x1) && mode > 0) {
				cpsr.reg &= 0x00ffffff;
				cpsr.reg |= ((operand >> 24) & 0xff) << 24;
			}
		}
		else {
			if (((opcode >> 16) & 0x1) && mode > 0) {
				setSPSR(getSPSR().reg & 0xffffff00);
				cpsr.reg |= (operand & 0xff);
			}
			else if (((opcode >> 17) & 0x1) && mode > 0) {
				setSPSR(getSPSR().reg & 0xffff00ff);
				cpsr.reg |= ((operand >> 8) & 0xff) << 8;
			}
			else if (((opcode >> 18) & 0x1) && mode > 0) {
				setSPSR(getSPSR().reg & 0xff00ffff);
				cpsr.reg |= ((operand >> 16) & 0xff) << 16;
			}
			else if (((opcode >> 19) & 0x1) && mode > 0) {
				setSPSR(getSPSR().reg & 0x00ffffff);
				cpsr.reg |= ((operand >> 24) & 0xff) << 24;
			}
		}
	}
	return 1;
}

uint32_t ARM7TDMI::ADD() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, Rn + shifter_operand);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = (Rd == 0);
				cpsr.C = (Rn + shifter_operand) < Rn;
				cpsr.V = (Rn >> 31) == (shifter_operand >> 31)
					&& ((Rn + shifter_operand) >> 31) != (Rn >> 31);
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::ADC() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, Rn + shifter_operand + cpsr.C);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = (Rd == 0);
				cpsr.C = (Rn + shifter_operand + cpsr.C) < Rn || (Rn + shifter_operand + cpsr.C) < shifter_operand;
				cpsr.V = (Rn >> 31) == (shifter_operand >> 31)
					  && ((Rn + shifter_operand + cpsr.C) >> 31) != (Rn >> 31);
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::SUB() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, Rn - shifter_operand);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = (Rd == 0);
				cpsr.C = (Rn >= shifter_operand);
				cpsr.V = (Rn >> 31) != (shifter_operand >> 31)
					&& ((Rn - shifter_operand) >> 31) == (shifter_operand >> 31);
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::SBC() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, Rn - shifter_operand - ~cpsr.C);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = (Rd == 0);
				cpsr.C = (Rn >= shifter_operand + ~cpsr.C);
				cpsr.V = (Rn >> 31) != (shifter_operand >> 31)
					&& ((Rn - shifter_operand - ~cpsr.C) >> 31) == (shifter_operand >> 31);
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::RSB() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, shifter_operand - Rn);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = (Rd == 0);
				cpsr.C = (shifter_operand >= Rn);
				cpsr.V = (Rn >> 31) != (shifter_operand >> 31)
					&& ((shifter_operand - Rn) >> 31) == (Rn >> 31);
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::RSC() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, shifter_operand - Rn - ~cpsr.C);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = (Rd == 0);
				cpsr.C = (shifter_operand >= Rn + ~cpsr.C);
				cpsr.V = (Rn >> 31) != (shifter_operand >> 31)
					&& ((shifter_operand - Rn - ~cpsr.C) >> 31) == (Rn >> 31);
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::MUL() {
	if (conditionPassed()) {
		uint32_t Rm = readRegister(opcode & 0xf);
		uint32_t Rs = readRegister((opcode >> 8) & 0xf);
		writeRegister((opcode >> 16) & 0xf, Rm * Rs);
		uint32_t Rd = readRegister((opcode >> 16) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			cpsr.N = Rd >> 31;
			cpsr.Z = (Rd == 0);
			//maybe this architecture version needs to modify C
			//but idk
		}
		return 1 + getMulIter(Rs);
	}
	return 1;
}
uint32_t ARM7TDMI::MLA() {
	if (conditionPassed()) {
		uint32_t Rm = readRegister(opcode & 0xf);
		uint32_t Rs = readRegister((opcode >> 8) & 0xf);
		uint32_t Rn = readRegister((opcode >> 12) & 0xf);
		writeRegister((opcode >> 16) & 0xf, Rm * Rs + Rn);
		uint32_t Rd = readRegister((opcode >> 16) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			cpsr.N = Rd >> 31;
			cpsr.Z = (Rd == 0);
			//maybe this architecture version needs to modify C&V
			//but idk
		}
		return 2 + getMulIter(Rs);
	}
	return 1;
}
uint32_t ARM7TDMI::UMULL() {
	if (conditionPassed()) {
		uint32_t Rm = readRegister(opcode & 0xf);
		uint32_t Rs = readRegister((opcode >> 8) & 0xf);

		uint64_t mul = (uint64_t) Rm * (uint64_t) Rs;
		uint32_t RdLo = (mul & 0xffffffff);
		uint32_t RdHi = (mul >> 32);

		writeRegister((opcode >> 16) & 0xf, RdHi);
		writeRegister((opcode >> 12) & 0xf, RdLo);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			cpsr.N = RdHi >> 31;
			cpsr.Z = RdHi == 0 && RdLo == 0;
			//maybe this architecture version needs to modify C
			//but idk
		}
		return 2 + getMulIter(Rs);
	}
	return 1;
}
uint32_t ARM7TDMI::UMLAL() {
	if (conditionPassed()) {
		uint32_t Rm = readRegister(opcode & 0xf);
		uint32_t Rs = readRegister((opcode >> 8) & 0xf);

		uint64_t mul = (uint64_t)Rm * (uint64_t)Rs;
		uint32_t RdLo = (mul & 0xffffffff) + readRegister((opcode >> 12) & 0xf);
		uint32_t RdHi = (mul >> 32) + readRegister((opcode >> 16) & 0xf) + (RdLo < (mul & 0xffffffff));

		writeRegister((opcode >> 16) & 0xf, RdHi);
		writeRegister((opcode >> 12) & 0xf, RdLo);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			cpsr.N = RdHi >> 31;
			cpsr.Z = RdHi == 0 && RdLo == 0;
			//maybe this architecture version needs to modify C
			//but idk
		}
		return 3 + getMulIter(Rs);
	}
	return 1;
}
uint32_t ARM7TDMI::SMULL() {
	if (conditionPassed()) {
		uint32_t Rm = readRegister(opcode & 0xf);
		uint32_t Rs = readRegister((opcode >> 8) & 0xf);

		uint64_t mul = (uint64_t)Rm * (uint64_t)Rs;
		uint32_t RdLo = (mul & 0xffffffff);
		uint32_t RdHi = (mul >> 32);

		writeRegister((opcode >> 16) & 0xf, RdHi);
		writeRegister((opcode >> 12) & 0xf, RdLo);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			cpsr.N = RdHi >> 31;
			cpsr.Z = RdHi == 0 && RdLo == 0;
			//maybe this architecture version needs to modify C
			//but idk
		}
		return 2 + getMulIter(Rs);
	}
	return 1;
}
uint32_t ARM7TDMI::SMLAL() {
	if (conditionPassed()) {
		uint32_t Rm = readRegister(opcode & 0xf);
		uint32_t Rs = readRegister((opcode >> 8) & 0xf);

		uint64_t mul = (uint64_t)Rm * (uint64_t)Rs;
		uint32_t RdLo = (mul & 0xffffffff) + readRegister((opcode >> 12) & 0xf);
		uint32_t RdHi = (mul >> 32) + readRegister((opcode >> 16) & 0xf) + (RdLo < (mul & 0xffffffff));

		writeRegister((opcode >> 16) & 0xf, RdHi);
		writeRegister((opcode >> 12) & 0xf, RdLo);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			cpsr.N = RdHi >> 31;
			cpsr.Z = RdHi == 0 && RdLo == 0;
			//maybe this architecture version needs to modify C
			//but idk
		}
		return 3 + getMulIter(Rs);
	}
	return 1;
}
uint32_t ARM7TDMI::CMP() {
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		uint32_t alu_out = Rn - shifter_operand;
		cpsr.N = alu_out >> 31;
		cpsr.Z = alu_out == 0;
		cpsr.C = Rn >= shifter_operand;
		cpsr.V = (Rn >> 31) != (shifter_operand >> 31)
			&& ((Rn - shifter_operand) >> 31) == (shifter_operand >> 31);
	}
	return 1;
}
uint32_t ARM7TDMI::CMN() {
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		uint32_t alu_out = Rn + shifter_operand;
		cpsr.N = alu_out >> 31;
		cpsr.Z = alu_out == 0;
		cpsr.C = (Rn + shifter_operand) < Rn;
		cpsr.V = (Rn >> 31) == (shifter_operand >> 31)
			&& ((Rn + shifter_operand) >> 31) != (Rn >> 31);
	}
	return 1;
}

uint32_t ARM7TDMI::TST() {
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		uint32_t alu_out = Rn & shifter_operand;
		cpsr.N = alu_out >> 31;
		cpsr.Z = alu_out == 0;
		cpsr.C = shifter_carry_out;
	}
	return 1;
}
uint32_t ARM7TDMI::TEQ() {
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		uint32_t alu_out = Rn ^ shifter_operand;
		cpsr.N = alu_out >> 31;
		cpsr.Z = alu_out == 0;
		cpsr.C = shifter_carry_out;
	}
	return 1;
}
uint32_t ARM7TDMI::AND() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, Rn & shifter_operand);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = Rd == 0;
				cpsr.C = shifter_carry_out;
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::EOR() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, Rn ^ shifter_operand);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = Rd == 0;
				cpsr.C = shifter_carry_out;
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::ORR() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, Rn | shifter_operand);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = Rd == 0;
				cpsr.C = shifter_carry_out;
			}
		}
	}
	return add_cycles;
}
uint32_t ARM7TDMI::BIC() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		writeRegister((opcode >> 12) & 0xf, Rn & ~shifter_operand);
		uint32_t Rd = readRegister((opcode >> 12) & 0xf);
		if (((opcode >> 20) & 0x1) == 1) { //if S==1
			if (((opcode >> 12) & 0xf) == 15) {
				cpsr = getSPSR();
				add_cycles = 2;
			}
			else {
				cpsr.N = Rd >> 31;
				cpsr.Z = Rd == 0;
				cpsr.C = shifter_carry_out;
			}
		}
	}
	return add_cycles;
}

uint32_t ARM7TDMI::B() {
	if (conditionPassed()) {
		uint32_t sign_extended = signExtend(opcode, 24);
		if (((opcode >> 24) & 0x1) == 1) writeRegister(14, prefetch); 
		pc += (sign_extended << 2);
		return 3;
	}
	return 1;
}

uint32_t ARM7TDMI::BX() {
	if (conditionPassed()) {
		uint32_t Rm = readRegister(opcode & 0xf);
		cpsr.T = Rm & 0x1;
		pc = Rm & 0xfffffffe;
		return 3;
	}
	return 1;
}

uint32_t ARM7TDMI::LDR() {
	if (conditionPassed()) {
		uint32_t value = 0;
		if ((ls_addr & 0b11) == 0b00) {
			value = read(ls_addr);
		}
		else if ((ls_addr & 0b11) == 0b01) {
			value = rotateRight(read(ls_addr), 8);
		}
		else if ((ls_addr & 0b11) == 0b10) {
			value = rotateRight(read(ls_addr), 16);
		}
		else {
			value = rotateRight(read(ls_addr), 24);
		}

		if (((opcode >> 12) & 0xf) == 15) { //Rd == pc
			pc = value & 0xfffffffc;
			return 5;
		}
		else {
			writeRegister((opcode >> 12) & 0xf, value);
			return 3;
		}
	}
	return 1;
}
uint32_t ARM7TDMI::LDRB() {
	if (conditionPassed()) {
		writeRegister((opcode >> 12) & 0xf, read(ls_addr, 8));
		return ((opcode >> 12) & 0xf) == 15 ? 5 : 3;
	}
	return 1;
}
uint32_t ARM7TDMI::LDRBT() {
	if (conditionPassed()) {
		writeRegister((opcode >> 12) & 0xf, read(ls_addr, 8));
		return ((opcode >> 12) & 0xf) == 15 ? 5 : 3;
	}
	return 1;
}
uint32_t ARM7TDMI::LDRT() {
	if (conditionPassed()) {
		uint32_t value = 0;
		if ((ls_addr & 0b11) == 0b00) {
			value = read(ls_addr);
		}
		else if ((ls_addr & 0b11) == 0b01) {
			value = rotateRight(read(ls_addr), 8);
		}
		else if ((ls_addr & 0b11) == 0b10) {
			value = rotateRight(read(ls_addr), 16);
		}
		else {
			value = rotateRight(read(ls_addr), 24);
		}
		writeRegister((opcode >> 12) & 0xf, value);
		return ((opcode >> 12) & 0xf) == 15 ? 5 : 3;
	}
	return 1;
}
uint32_t ARM7TDMI::LDRH() {
	if (conditionPassed()) {
		writeRegister((opcode >> 12) & 0xf, read(ls_addr, 16));
		return ((opcode >> 12) & 0xf) == 15 ? 5 : 3;
	}
	return 1;
}
uint32_t ARM7TDMI::LDRSB() {
	if (conditionPassed()) {
		uint32_t d = read(ls_addr, 8);
		writeRegister((opcode >> 12) & 0xf, signExtend(d, 8));
		return ((opcode >> 12) & 0xf) == 15 ? 5 : 3;
	}
	return 1;
}
uint32_t ARM7TDMI::LDRSH() {
	if (conditionPassed()) {
		uint32_t d = read(ls_addr, 16);
		writeRegister((opcode >> 12) & 0xf, signExtend(d, 16));
		return ((opcode >> 12) & 0xf) == 15 ? 5 : 3;
	}
	return 1;
}
uint32_t ARM7TDMI::LDM_1() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		add_cycles = 2;
		uint32_t a = ls_start_addr;
		for (auto i = 0; i < 15; ++i) {
			if ((opcode >> i) == 1) {
				writeRegister(i, read(a));
				a += 4;
				++add_cycles;
			}
		}
		if ((opcode >> 15) == 1) {
			pc = read(a) & 0xfffffffc;
			add_cycles += 2;
		}
		//a should be = ls_end_addr by now
	}
	return add_cycles;
}
uint32_t ARM7TDMI::LDM_2() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		add_cycles = 2;
		uint32_t a = ls_start_addr;
		for (auto i = 0; i < 15; ++i) {
			if ((opcode >> i) == 1) {
				writeRegister(i, read(a), 0); //user mode registers
				a += 4;
				++add_cycles;
			}
		}
		//a should be = ls_end_addr by now
	}
	return add_cycles;
}
uint32_t ARM7TDMI::LDM_3() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		add_cycles = 2;
		uint32_t a = ls_start_addr;
		for (auto i = 0; i < 15; ++i) {
			if ((opcode >> i) == 1) {
				writeRegister(i, read(a));
				a += 4;
				++add_cycles;
			}
		}
		cpsr = getSPSR();
		if (cpsr.T == 1) {
			pc = read(a) & 0xfffffffe;
		}
		else {
			pc = read(a) & 0xfffffffc;
		}
		add_cycles += 2;
		//a should be = ls_end_addr by now
	}
	return add_cycles;
}
uint32_t ARM7TDMI::STR() {
	if (conditionPassed()) {
		write(ls_addr, readRegister((opcode >> 12) & 0xf));
		return 2;
	}
	return 1;
}
uint32_t ARM7TDMI::STRT() {
	if (conditionPassed()) {
		write(ls_addr, readRegister((opcode >> 12) & 0xf));
		return 2;
	}
	return 1;
}
uint32_t ARM7TDMI::STRB() {
	if (conditionPassed()) {
		write(ls_addr, readRegister((opcode >> 12) & 0xf) & 0xff, 8);
		return 2;
	}
	return 1;
}
uint32_t ARM7TDMI::STRBT() {
	if (conditionPassed()) {
		write(ls_addr, readRegister((opcode >> 12) & 0xf) & 0xff, 8);
		return 2;
	}
	return 1;
}
uint32_t ARM7TDMI::STRH() {
	if (conditionPassed()) {
		if ((ls_addr & 0x1) == 0) {
			write(ls_addr, readRegister((opcode >> 12) & 0xf) & 0xffff, 16);
		}
		return 2;
	}
	return 1;
}
uint32_t ARM7TDMI::STM_1() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t a = ls_start_addr;
		for (auto i = 0; i < 16; ++i) {
			if ((opcode >> i) == 1) {
				write(a, readRegister(i));
				a += 4;
				++add_cycles;
			}
		}
		//a should be = ls_end_addr by now
	}
	return add_cycles;
}
uint32_t ARM7TDMI::STM_2() {
	uint32_t add_cycles = 1;
	if (conditionPassed()) {
		uint32_t a = ls_start_addr;
		for (auto i = 0; i < 16; ++i) {
			if ((opcode >> i) == 1) {
				write(a, readRegister(i, 0));
				a += 4;
				++add_cycles;
			}
		}
		//a should be = ls_end_addr by now
	}
	return add_cycles;
}

uint32_t ARM7TDMI::SWP() {
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		uint32_t temp = 0;
		if ((Rn & 0b11) == 0b00) {
			temp = read(Rn);
		}
		else if ((Rn & 0b11) == 0b01) {
			temp = rotateRight(read(Rn), 8);
		} 
		else if ((Rn & 0b11) == 0b10) {
			temp = rotateRight(read(Rn), 16);
		}
		else {
			temp = rotateRight(read(Rn), 24);
		}
		write(Rn, readRegister((opcode & 0xf)));
		writeRegister((opcode >> 12) & 0xf, temp);
		return 4;
	}
	return 1;
}
uint32_t ARM7TDMI::SWPB() {
	if (conditionPassed()) {
		uint32_t Rn = readRegister((opcode >> 16) & 0xf);
		uint32_t Rm = readRegister(opcode & 0xf);
		uint32_t temp = read(Rn, 8);
		write(Rn, Rm & 0xff, 8);
		writeRegister((opcode >> 12) & 0xf, temp);
		return 4;
	}
	return 1;
}

uint32_t ARM7TDMI::SWI() {
	if (conditionPassed()) {
		writeRegister(14, prefetch, 2);
		mode = 2;
		setSPSR(cpsr.reg);
		cpsr.M = 0b10011;
		cpsr.T = 0;
		cpsr.I = 1;
		pc = 0x00000008; //fix later
	}
	return 1;
}

uint32_t ARM7TDMI::NOP() {
	return 1;
}

void ARM7TDMI::disassembleARM(const std::vector<uint32_t>& mem)
{
	for (auto i = 0; i < mem.size(); ++i) {
		if (mem[i] == 0) break;
		std::printf("%04X\t%08X\t%s\n", i * 4, mem[i], disassembleARMInstruction(mem[i], i * 4).c_str());
		/*std::cout << std::setw(8) << std::hex << std::ios::right << mem[i]; 
		std::cout << '\t' << disassembleARMInstruction(mem[i], i * 4) << '\n';*/
	}
}

std::string ARM7TDMI::disassembleARMInstruction(const uint32_t instruction32, const uint32_t addr)
{
	uint32_t row = getBits(instruction32, 20, 8); //bits 27-20
	uint32_t col = getBits(instruction32, 4, 4); //bits 7-4

	Instruction instruction = instruction_lookup[row][col];
	CONDITION condition = condition_lookup[getBits(instruction32, 28, 4)];
	std::ostringstream str_instruction;
	if (instruction.name == "???") {
		str_instruction << "Undefined instruction";
	} else {
		uint32_t tmp = 0; //for convenience
		std::string name = "", suffix = "";
		switch (instruction.addrmode->group) {
		case AddrModeGroup::AM_MODE1:
			if (instruction.name.back() == 'S') {
				name = instruction.name.substr(0, instruction.name.length() - 1);
				suffix = "S";
			}
			else
				name = instruction.name;
			break;
		case AddrModeGroup::AM_MODE2:
			if (instruction.name.substr(instruction.name.length() - 2, 2) == "BT") {
				name = instruction.name.substr(0, instruction.name.length() - 2);
				suffix = "BT";
			}
			else if (instruction.name.back() == 'B' || instruction.name.back() == 'T') {
				name = instruction.name.substr(0, instruction.name.length() - 1);
				suffix = instruction.name.back();
			}
			else
				name = instruction.name;
			break;
		case AddrModeGroup::AM_MODE3:
			if (instruction.name.substr(instruction.name.length() - 2) == "SH" ||
				instruction.name.substr(instruction.name.length() - 2) == "SB") {
				name = instruction.name.substr(0, instruction.name.length() - 2);
				suffix = instruction.name.substr(instruction.name.length() - 2);
			}
			else if (instruction.name.back() == 'H' || instruction.name.back() == 'D') {
				name = instruction.name.substr(0, instruction.name.length() - 1);
				suffix = instruction.name.back();
			}
			else
				name = instruction.name;
			break;
		case AddrModeGroup::AM_MODE4:
			name = instruction.name.substr(0, instruction.name.length() - 2);
			suffix = instruction.name.substr(instruction.name.length() - 2);
			break;
		case AddrModeGroup::AM_NOTHING:
			name = instruction.name;
			break;
		default:
			break;
		}
		// instruction name & condition
		str_instruction << name;
		// condition field
		if (condition.name != "AL")
			str_instruction << condition.name;
		// add suffix
		str_instruction << suffix << ' ';

		// process common syntax (Rd, Rn...)
		switch (instruction.addrmode->group) {
		case AddrModeGroup::AM_MODE1:
			if (!(name == "CMN" || name == "CMP" ||
				name == "TEQ" || name == "TST")) {
				str_instruction << getRegisterName(getBits(instruction32, 12, 4)) << ", ";
			}
			if (!(name == "MOV" || name == "MVN")) {
				str_instruction << getRegisterName(getBits(instruction32, 16, 4)) << ", ";
			}
			break;
		case AddrModeGroup::AM_MODE2:
			str_instruction << getRegisterName(getBits(instruction32, 12, 4)); //Rd
			str_instruction << ", [" << getRegisterName(getBits(instruction32, 16, 4)); //Rn
			break;
		case AddrModeGroup::AM_MODE3:
			str_instruction << getRegisterName(getBits(instruction32, 12, 4)); //Rd
			str_instruction << ", [" << getRegisterName(getBits(instruction32, 16, 4)); //Rn
			tmp = (getBits(instruction32, 8, 4) << 4) | getBits(instruction32, 0, 4);
			break;
		case AddrModeGroup::AM_MODE4:
		{
			str_instruction << getRegisterName(getBits(instruction32, 16, 4))
				<< (getBit(instruction32, 21) ? "!" : "")
				<< ", {";
			bool firstReg = true;
			for (int i = 0; i < 16; ++i) {
				if (getBit(instruction32, i)) {
					if (!firstReg) {
						str_instruction << ", ";
					}
					else firstReg = false;
					str_instruction << getRegisterName(i);
				}
			}
			str_instruction << '}';
		}
			break;
		case AddrModeGroup::AM_NOTHING:
			if (name == "B" || name == "BL")
				str_instruction << "0x" << std::hex
								<< ((signExtend(getBits(instruction32, 0, 24), 24) << 2) + addr + 8);
			else if (name == "BX")
				str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			else if (name == "MRS") {
				str_instruction << getRegisterName(getBits(instruction32, 12, 4)) << ", ";
				if (getBit(instruction32, 22))
					str_instruction << "SPSR";
				else 
					str_instruction << "CPSR";
			}
			else if (name == "MSR") {
				if (getBit(instruction32, 22))
					str_instruction << "SPSR_";
				else
					str_instruction << "CPSR_";
				if (getBit(instruction32, 19)) str_instruction << 'f';
				if (getBit(instruction32, 18)) str_instruction << 's';
				if (getBit(instruction32, 17)) str_instruction << 'x';
				if (getBit(instruction32, 16)) str_instruction << 'c';
				str_instruction << ", ";
				if (getBit(instruction32, 25))
					str_instruction << '#' << rotateRight(getBits(instruction32, 0, 8),
														getBits(instruction32, 8, 4) * 2);
				else
					str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			}
			else if (name == "MUL") {
				str_instruction << getRegisterName(getBits(instruction32, 16, 4))
					<< ", " << getRegisterName(getBits(instruction32, 0, 4))
					<< ", " << getRegisterName(getBits(instruction32, 8, 4));
			}
			else if (name == "MLA") {
				str_instruction << getRegisterName(getBits(instruction32, 16, 4))
					<< ", " << getRegisterName(getBits(instruction32, 0, 4))
					<< ", " << getRegisterName(getBits(instruction32, 8, 4))
					<< ", " << getRegisterName(getBits(instruction32, 12, 4));
			}
			else if (contains(name, "MUL") || contains(name, "MLA")) {
				str_instruction << getRegisterName(getBits(instruction32, 12, 4))
					<< ", " << getRegisterName(getBits(instruction32, 16, 4))
					<< ", " << getRegisterName(getBits(instruction32, 0, 4))
					<< ", " << getRegisterName(getBits(instruction32, 8, 4));
			}
			break;
		default:
			break;
		}

		switch (instruction.addrmode->name) {
		case AddrModeName::m1_ARI:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			str_instruction << ", ASR #" << getBits(instruction32, 7, 5);
			break;
		case AddrModeName::m1_ARR:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			str_instruction << ", ASR " << getRegisterName(getBits(instruction32, 8, 4));
			break;
		case AddrModeName::m1_IMM:
			str_instruction << "#" << rotateRight(getBits(instruction32, 0, 8),
													getBits(instruction32, 8, 4) * 2);
			break;
		case AddrModeName::m1_LLI:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			str_instruction << ", LSL #" << getBits(instruction32, 7, 5);
			break;
		case AddrModeName::m1_LLR:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			str_instruction << ", LSL " << getRegisterName(getBits(instruction32, 8, 4));
			break;
		case AddrModeName::m1_LRI:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			str_instruction << ", LSR #" << getBits(instruction32, 7, 5);
			break;
		case AddrModeName::m1_LRR:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			str_instruction << ", LSR " << getRegisterName(getBits(instruction32, 8, 4));
			break;
		case AddrModeName::m1_REG:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			break;
		case AddrModeName::m1_RRI:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			str_instruction << ", ROR #" << getBits(instruction32, 7, 5);
			break;
		case AddrModeName::m1_RRR:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			str_instruction << ", ROR " << getRegisterName(getBits(instruction32, 8, 4));
			break;
		case AddrModeName::m1_RRX:
			str_instruction << getRegisterName(getBits(instruction32, 0, 4));
			str_instruction << ", RRX";
			break;
		case AddrModeName::m2_IMO:
			if (getBits(instruction32, 0, 12) != 0) {
				str_instruction << ", #"
					<< (getBit(instruction32, 23) ? "" : "-")
					<< getBits(instruction32, 0, 12) << ']';
			}
			break;
		case AddrModeName::m2_IMP:
			if (getBits(instruction32, 0, 12) != 0) {
				str_instruction << "], #"
					<< (getBit(instruction32, 23) ? "" : "-")
					<< getBits(instruction32, 0, 12);
			}
			break;
		case AddrModeName::m2_PIM:
			if (getBits(instruction32, 0, 12) != 0) {
				str_instruction << ", #"
					<< (getBit(instruction32, 23) ? "" : "-")
					<< getBits(instruction32, 0, 12) << ']'
					<< '!';
			}
			break;
		case AddrModeName::m2_PRG:
			str_instruction << ", "
				<< (getBit(instruction32, 23) ? "" : "-")
				<< getRegisterName(getBits(instruction32, 0, 4)) << ']'
				<< '!';
			break;
		case AddrModeName::m2_PSR:
			str_instruction << ", "
				<< (getBit(instruction32, 23) ? "" : "-")
				<< getRegisterName(getBits(instruction32, 0, 4))
				<< ", "
				<< parseShiftIMM(instruction32) << "]!";
			break;
		case AddrModeName::m2_RGO:
			str_instruction << ", "
				<< (getBit(instruction32, 23) ? "" : "-")
				<< getRegisterName(getBits(instruction32, 0, 4)) << ']';
			break;
		case AddrModeName::m2_RGP:
			str_instruction << ", "
				<< (getBit(instruction32, 23) ? "" : "-")
				<< getRegisterName(getBits(instruction32, 0, 4)) << ']';
			break;
		case AddrModeName::m2_SRO:
			str_instruction << ", "
				<< (getBit(instruction32, 23) ? "" : "-")
				<< getRegisterName(getBits(instruction32, 0, 4))
				<< ", "
				<< parseShiftIMM(instruction32) << ']';
			break;
		case AddrModeName::m2_SRP:
			str_instruction << "], "
				<< (getBit(instruction32, 23) ? "" : "-")
				<< getRegisterName(getBits(instruction32, 0, 4)) //Rm
				<< ", "
				<< parseShiftIMM(instruction32);
			break;
		case AddrModeName::m3_IMO:
			if (tmp)
				str_instruction << ", #"
								<< (getBit(instruction32, 23) ? "" : "-") << tmp;
			str_instruction << ']';
			break;
		case AddrModeName::m3_IMP:
			str_instruction << "], #"
							<< (getBit(instruction32, 23) ? "" : "-") << tmp;
			break;
		case AddrModeName::m3_PIM:
			str_instruction << ", #"
							<< (getBit(instruction32, 23) ? "" : "-") << tmp
							<< "]!";
			break;
		case AddrModeName::m3_PRG:
			str_instruction << ", "
							<< (getBit(instruction32, 23) ? "" : "-") 
							<< getRegisterName(getBits(instruction32, 0, 4))
							<< "]!";
			break;
		case AddrModeName::m3_RGO:
			str_instruction << ", "
							<< (getBit(instruction32, 23) ? "" : "-") 
							<< getRegisterName(getBits(instruction32, 0, 4))
							<< ']';
			break;
		case AddrModeName::m3_RGP:
			str_instruction << "], "
							<< (getBit(instruction32, 23) ? "" : "-")
							<< getRegisterName(getBits(instruction32, 0, 4));
			break;
		case AddrModeName::m4_IA:
		case AddrModeName::m4_IB:
		case AddrModeName::m4_DA:
		case AddrModeName::m4_DB:
		default:
			break;
		}
	}
	return str_instruction.str();
}

ARM7TDMI::Instruction ARM7TDMI::getInstruction(uint32_t opc)
{
	return instruction_lookup[getBits(opc, 20, 8)][getBits(opc, 4, 4)];
}

std::string ARM7TDMI::parseShiftIMM(const uint32_t instruction32) {
	std::ostringstream ret;
	switch (getBits(instruction32, 4, 3)) {
	case 0b000:
		ret << "LSL #" << getBits(instruction32, 7, 5);
		break;
	case 0b010:
		ret << "LSR #" << getBits(instruction32, 7, 5);
		break;
	case 0b100:
		ret << "ASR #" << getBits(instruction32, 7, 5);
		break;
	case 0b110:
		if (getBits(instruction32, 7, 5) == 0)
			ret << "RRX";
		else
			ret << "ROR " << getBits(instruction32, 7, 5);
		break;
	default:
		break;
	}
	return ret.str();
}
