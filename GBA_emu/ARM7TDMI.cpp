#include "ARM7TDMI.h"

ARM7TDMI::ARM7TDMI() {
	using a = ARM7TDMI;
	cond_lookup = {
		{"EQ", &a::EQ}, {"NE", &a::NE}, {"CS", &a::CS}, {"CC", &a::CC}, {"MI", &a::MI},
		{"PL", &a::PL}, {"VS", &a::VS}, {"VC", &a::VC}, {"HI", &a::HI}, {"LS", &a::LS}, 
		{"GE", &a::GE}, {"LT", &a::LT}, {"GT", &a::GT}, {"LE", &a::LE}, {"AL", &a::AL},
		{"NV", &a::NV},
	};

	lookup = {
		{{"AND", &a::AND, &a::m1_LLI},  {"AND", &a::AND, &a::m1_LLR},  {"AND", &a::AND, &a::m1_LRI},  {"AND", &a::AND, &a::m1_LRR},  {"AND", &a::AND, &a::m1_ARI},  {"AND", &a::AND, &a::m1_ARR},  {"AND", &a::AND, &a::m1_RRI},  {"AND", &a::AND, &a::m1_RRR},  {"AND", &a::AND, &a::m1_LLI},  {"MUL", &a::MUL, &a::XXX},      {"AND", &a::AND, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_RGP}, {"AND", &a::AND, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"AND", &a::AND, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"ANDS", &a::AND, &a::m1_LLI}, {"ANDS", &a::AND, &a::m1_LLR}, {"ANDS", &a::AND, &a::m1_LRI}, {"ANDS", &a::AND, &a::m1_LRR}, {"ANDS", &a::AND, &a::m1_ARI}, {"ANDS", &a::AND, &a::m1_ARR}, {"ANDS", &a::AND, &a::m1_RRI}, {"ANDS", &a::AND, &a::m1_RRR}, {"ANDS", &a::AND, &a::m1_LLI}, {"MULS", &a::MUL, &a::XXX},     {"ANDS", &a::AND, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_RGP}, {"ANDS", &a::AND, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_RGP}, {"ANDS", &a::AND, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_RGP}},
		{{"EOR", &a::EOR, &a::m1_LLI},  {"EOR", &a::EOR, &a::m1_LLR},  {"EOR", &a::EOR, &a::m1_LRI},  {"EOR", &a::EOR, &a::m1_LRR},  {"EOR", &a::EOR, &a::m1_ARI},  {"EOR", &a::EOR, &a::m1_ARR},  {"EOR", &a::EOR, &a::m1_RRI},  {"EOR", &a::EOR, &a::m1_RRR},  {"EOR", &a::EOR, &a::m1_LLI},  {"MLA", &a::MLA, &a::XXX},      {"EOR", &a::EOR, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_RGP}, {"EOR", &a::EOR, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"EOR", &a::EOR, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"EORS", &a::EOR, &a::m1_LLI}, {"EORS", &a::EOR, &a::m1_LLR}, {"EORS", &a::EOR, &a::m1_LRI}, {"EORS", &a::EOR, &a::m1_LRR}, {"EORS", &a::EOR, &a::m1_ARI}, {"EORS", &a::EOR, &a::m1_ARR}, {"EORS", &a::EOR, &a::m1_RRI}, {"EORS", &a::EOR, &a::m1_RRR}, {"EORS", &a::EOR, &a::m1_LLI}, {"MLAS", &a::MUL, &a::XXX},     {"EORS", &a::EOR, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_RGP}, {"EORS", &a::EOR, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_RGP}, {"EORS", &a::EOR, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_RGP}},
		{{"SUB", &a::SUB, &a::m1_LLI},  {"SUB", &a::SUB, &a::m1_LLR},  {"SUB", &a::SUB, &a::m1_LRI},  {"SUB", &a::SUB, &a::m1_LRR},  {"SUB", &a::SUB, &a::m1_ARI},  {"SUB", &a::SUB, &a::m1_ARR},  {"SUB", &a::SUB, &a::m1_RRI},  {"SUB", &a::SUB, &a::m1_RRR},  {"SUB", &a::SUB, &a::m1_LLI},  {"???", &a::NOP, &a::XXX},      {"SUB", &a::SUB, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_IMP}, {"SUB", &a::SUB, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"SUB", &a::SUB, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"SUBS", &a::SUB, &a::m1_LLI}, {"SUBS", &a::SUB, &a::m1_LLR}, {"SUBS", &a::SUB, &a::m1_LRI}, {"SUBS", &a::SUB, &a::m1_LRR}, {"SUBS", &a::SUB, &a::m1_ARI}, {"SUBS", &a::SUB, &a::m1_ARR}, {"SUBS", &a::SUB, &a::m1_RRI}, {"SUBS", &a::SUB, &a::m1_RRR}, {"SUBS", &a::SUB, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"SUBS", &a::SUB, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_IMP}, {"SUBS", &a::SUB, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_IMP}, {"SUBS", &a::SUB, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_IMP}},
		{{"RSB", &a::RSB, &a::m1_LLI},  {"RSB", &a::RSB, &a::m1_LLR},  {"RSB", &a::RSB, &a::m1_LRI},  {"RSB", &a::RSB, &a::m1_LRR},  {"RSB", &a::RSB, &a::m1_ARI},  {"RSB", &a::RSB, &a::m1_ARR},  {"RSB", &a::RSB, &a::m1_RRI},  {"RSB", &a::RSB, &a::m1_RRR},  {"RSB", &a::RSB, &a::m1_LLI},  {"???", &a::NOP, &a::XXX},	    {"RSB", &a::RSB, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_IMP}, {"RSB", &a::RSB, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"RSB", &a::RSB, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"RSBS", &a::RSB, &a::m1_LLI}, {"RSBS", &a::RSB, &a::m1_LLR}, {"RSBS", &a::RSB, &a::m1_LRI}, {"RSBS", &a::RSB, &a::m1_LRR}, {"RSBS", &a::RSB, &a::m1_ARI}, {"RSBS", &a::RSB, &a::m1_ARR}, {"RSBS", &a::RSB, &a::m1_RRI}, {"RSBS", &a::RSB, &a::m1_RRR}, {"RSBS", &a::RSB, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"RSBS", &a::RSB, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_IMP}, {"RSBS", &a::RSB, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_IMP}, {"RSBS", &a::RSB, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_IMP}},
		{{"ADD", &a::ADD, &a::m1_LLI},  {"ADD", &a::ADD, &a::m1_LLR},  {"ADD", &a::ADD, &a::m1_LRI},  {"ADD", &a::ADD, &a::m1_LRR},  {"ADD", &a::ADD, &a::m1_ARI},  {"ADD", &a::ADD, &a::m1_ARR},  {"ADD", &a::ADD, &a::m1_RRI},  {"ADD", &a::ADD, &a::m1_RRR},  {"ADD", &a::ADD, &a::m1_LLI},  {"UMULL", &a::UMULL, &a::XXX},  {"ADD", &a::ADD, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_RGP}, {"ADD", &a::ADD, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"ADD", &a::ADD, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"ADDS", &a::ADD, &a::m1_LLI}, {"ADDS", &a::ADD, &a::m1_LLR}, {"ADDS", &a::ADD, &a::m1_LRI}, {"ADDS", &a::ADD, &a::m1_LRR}, {"ADDS", &a::ADD, &a::m1_ARI}, {"ADDS", &a::ADD, &a::m1_ARR}, {"ADDS", &a::ADD, &a::m1_RRI}, {"ADDS", &a::ADD, &a::m1_RRR}, {"ADDS", &a::ADD, &a::m1_LLI}, {"UMULLS", &a::UMULL, &a::XXX}, {"ADDS", &a::ADD, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_RGP}, {"ADDS", &a::ADD, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_RGP}, {"ADDS", &a::ADD, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_RGP}},
		{{"ADC", &a::ADC, &a::m1_LLI},  {"ADC", &a::ADC, &a::m1_LLR},  {"ADC", &a::ADC, &a::m1_LRI},  {"ADC", &a::ADC, &a::m1_LRR},  {"ADC", &a::ADC, &a::m1_ARI},  {"ADC", &a::ADC, &a::m1_ARR},  {"ADC", &a::ADC, &a::m1_RRI},  {"ADC", &a::ADC, &a::m1_RRR},  {"ADC", &a::ADC, &a::m1_LLI},  {"UMLAL", &a::UMLAL, &a::XXX},  {"ADC", &a::ADC, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_RGP}, {"ADC", &a::ADC, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"ADC", &a::ADC, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"ADCS", &a::ADC, &a::m1_LLI}, {"ADCS", &a::ADC, &a::m1_LLR}, {"ADCS", &a::ADC, &a::m1_LRI}, {"ADCS", &a::ADC, &a::m1_LRR}, {"ADCS", &a::ADC, &a::m1_ARI}, {"ADCS", &a::ADC, &a::m1_ARR}, {"ADCS", &a::ADC, &a::m1_RRI}, {"ADCS", &a::ADC, &a::m1_RRR}, {"ADCS", &a::ADC, &a::m1_LLI}, {"UMLALS", &a::UMLAL, &a::XXX}, {"ADCS", &a::ADC, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_RGP}, {"ADCS", &a::ADC, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_RGP}, {"ADCS", &a::ADC, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_RGP}},
		{{"SBC", &a::SBC, &a::m1_LLI},  {"SBC", &a::SBC, &a::m1_LLR},  {"SBC", &a::SBC, &a::m1_LRI},  {"SBC", &a::SBC, &a::m1_LRR},  {"SBC", &a::SBC, &a::m1_ARI},  {"SBC", &a::SBC, &a::m1_ARR},  {"SBC", &a::SBC, &a::m1_RRI},  {"SBC", &a::SBC, &a::m1_RRR},  {"SBC", &a::SBC, &a::m1_LLI},  {"SMULL", &a::SMULL, &a::XXX},  {"SBC", &a::SBC, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_IMP}, {"SBC", &a::SBC, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"SBC", &a::SBC, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"SBCS", &a::SBC, &a::m1_LLI}, {"SBCS", &a::SBC, &a::m1_LLR}, {"SBCS", &a::SBC, &a::m1_LRI}, {"SBCS", &a::SBC, &a::m1_LRR}, {"SBCS", &a::SBC, &a::m1_ARI}, {"SBCS", &a::SBC, &a::m1_ARR}, {"SBCS", &a::SBC, &a::m1_RRI}, {"SBCS", &a::SBC, &a::m1_RRR}, {"SBCS", &a::SBC, &a::m1_LLI}, {"SMULLS", &a::SMULL, &a::XXX}, {"SBCS", &a::SBC, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_IMP}, {"SBCS", &a::SBC, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_IMP}, {"SBCS", &a::SBC, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_IMP}},
		{{"RSC", &a::RSC, &a::m1_LLI},  {"RSC", &a::RSC, &a::m1_LLR},  {"RSC", &a::RSC, &a::m1_LRI},  {"RSC", &a::RSC, &a::m1_LRR},  {"RSC", &a::RSC, &a::m1_ARI},  {"RSC", &a::RSC, &a::m1_ARR},  {"RSC", &a::RSC, &a::m1_RRI},  {"RSC", &a::RSC, &a::m1_RRR},  {"RSC", &a::RSC, &a::m1_LLI},  {"SMLAL", &a::SMULL, &a::XXX},  {"RSC", &a::RSC, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_IMP}, {"RSC", &a::RSC, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"RSC", &a::RSC, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"RSCS", &a::RSC, &a::m1_LLI}, {"RSCS", &a::RSC, &a::m1_LLR}, {"RSCS", &a::RSC, &a::m1_LRI}, {"RSCS", &a::RSC, &a::m1_LRR}, {"RSCS", &a::RSC, &a::m1_ARI}, {"RSCS", &a::RSC, &a::m1_ARR}, {"RSCS", &a::RSC, &a::m1_RRI}, {"RSCS", &a::RSC, &a::m1_RRR}, {"RSCS", &a::RSC, &a::m1_LLI}, {"SMLALS", &a::SMULL, &a::XXX}, {"RSCS", &a::RSC, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_IMP}, {"RSCS", &a::RSC, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_IMP}, {"RSCS", &a::RSC, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_IMP}},
		{{"MRS", &a::MRS, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},	  {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"SWP", &a::SWP, &a::XXX},      {"???", &a::NOP, &a::XXX},     {"STRH", &a::STRH, &a::m3_RGO}, {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},        {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX}},
		{{"TSTS", &a::TST, &a::m1_LLI}, {"TSTS", &a::TST, &a::m1_LLR}, {"TSTS", &a::TST, &a::m1_LRI}, {"TSTS", &a::TST, &a::m1_LRR}, {"TSTS", &a::TST, &a::m1_ARI}, {"TSTS", &a::TST, &a::m1_ARR}, {"TSTS", &a::TST, &a::m1_RRI}, {"TSTS", &a::TST, &a::m1_RRR}, {"TSTS", &a::TST, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"TSTS", &a::TST, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_RGO}, {"TSTS", &a::TST, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_RGO}, {"TSTS", &a::TST, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_RGO}},
		{{"MSR", &a::MSR, &a::XXX},     {"BX", &a::BX, &a::XXX},       {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},      {"???", &a::NOP, &a::XXX},     {"STRH", &a::STRH, &a::m3_PRG}, {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},        {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX}},
		{{"TEQS", &a::TEQ, &a::m1_LLI}, {"TEQS", &a::TEQ, &a::m1_LLR}, {"TEQS", &a::TEQ, &a::m1_LRI}, {"TEQS", &a::TEQ, &a::m1_LRR}, {"TEQS", &a::TEQ, &a::m1_ARI}, {"TEQS", &a::TEQ, &a::m1_ARR}, {"TEQS", &a::TEQ, &a::m1_RRI}, {"TEQS", &a::TEQ, &a::m1_RRR}, {"TEQS", &a::TEQ, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"TEQS", &a::TEQ, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_PRG}, {"TEQS", &a::TEQ, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_PRG}, {"TEQS", &a::TEQ, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_PRG}},
		{{"MRS", &a::MRS, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},	  {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"SWPB", &a::SWPB, &a::XXX},    {"???", &a::NOP, &a::XXX},     {"STRH", &a::STRH, &a::m3_IMO}, {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},        {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX}},
		{{"CMPS", &a::CMP, &a::m1_LLI}, {"CMPS", &a::CMP, &a::m1_LLR}, {"CMPS", &a::CMP, &a::m1_LRI}, {"CMPS", &a::CMP, &a::m1_LRR}, {"CMPS", &a::CMP, &a::m1_ARI}, {"CMPS", &a::CMP, &a::m1_ARR}, {"CMPS", &a::CMP, &a::m1_RRI}, {"CMPS", &a::CMP, &a::m1_RRR}, {"CMPS", &a::CMP, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"CMPS", &a::CMP, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_IMO}, {"CMPS", &a::CMP, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_IMO}, {"CMPS", &a::CMP, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_IMO}},
		{{"MSR", &a::MSR, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},      {"???", &a::NOP, &a::XXX},     {"STRH", &a::STRH, &a::m3_PIM}, {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},        {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX}},
		{{"CMNS", &a::CMN, &a::m1_LLI}, {"CMNS", &a::CMN, &a::m1_LLR}, {"CMNS", &a::CMN, &a::m1_LRI}, {"CMNS", &a::CMN, &a::m1_LRR}, {"CMNS", &a::CMN, &a::m1_ARI}, {"CMNS", &a::CMN, &a::m1_ARR}, {"CMNS", &a::CMN, &a::m1_RRI}, {"CMNS", &a::CMN, &a::m1_RRR}, {"CMNS", &a::CMN, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"CMNS", &a::CMN, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_PIM}, {"CMNS", &a::CMN, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_PIM}, {"CMNS", &a::CMN, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_PIM}},
		{{"ORR", &a::ORR, &a::m1_LLI},  {"ORR", &a::ORR, &a::m1_LLR},  {"ORR", &a::ORR, &a::m1_LRI},  {"ORR", &a::ORR, &a::m1_LRR},  {"ORR", &a::ORR, &a::m1_ARI},  {"ORR", &a::ORR, &a::m1_ARR},  {"ORR", &a::ORR, &a::m1_RRI},  {"ORR", &a::ORR, &a::m1_RRR},  {"ORR", &a::ORR, &a::m1_LLI},  {"???", &a::NOP, &a::XXX},      {"ORR", &a::ORR, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_RGO}, {"ORR", &a::ORR, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"ORR", &a::ORR, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"ORRS", &a::ORR, &a::m1_LLI}, {"ORRS", &a::ORR, &a::m1_LLR}, {"ORRS", &a::ORR, &a::m1_LRI}, {"ORRS", &a::ORR, &a::m1_LRR}, {"ORRS", &a::ORR, &a::m1_ARI}, {"ORRS", &a::ORR, &a::m1_ARR}, {"ORRS", &a::ORR, &a::m1_RRI}, {"ORRS", &a::ORR, &a::m1_RRR}, {"ORRS", &a::ORR, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"ORRS", &a::ORR, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_RGO}, {"ORRS", &a::ORR, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_RGO}, {"ORRS", &a::ORR, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_RGO}},
		{{"MOV", &a::MOV, &a::m1_LLI},  {"MOV", &a::MOV, &a::m1_LLR},  {"MOV", &a::MOV, &a::m1_LRI},  {"MOV", &a::MOV, &a::m1_LRR},  {"MOV", &a::MOV, &a::m1_ARI},  {"MOV", &a::MOV, &a::m1_ARR},  {"MOV", &a::MOV, &a::m1_RRI},  {"MOV", &a::MOV, &a::m1_RRR},  {"MOV", &a::MOV, &a::m1_LLI},  {"???", &a::NOP, &a::XXX},      {"MOV", &a::MOV, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_PRG}, {"MOV", &a::MOV, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"MOV", &a::MOV, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"MOVS", &a::MOV, &a::m1_LLI}, {"MOVS", &a::MOV, &a::m1_LLR}, {"MOVS", &a::MOV, &a::m1_LRI}, {"MOVS", &a::MOV, &a::m1_LRR}, {"MOVS", &a::MOV, &a::m1_ARI}, {"MOVS", &a::MOV, &a::m1_ARR}, {"MOVS", &a::MOV, &a::m1_RRI}, {"MOVS", &a::MOV, &a::m1_RRR}, {"MOVS", &a::MOV, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"MOVS", &a::MOV, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_PRG}, {"MOVS", &a::MOV, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_PRG}, {"MOVS", &a::MOV, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_PRG}},
		{{"BIC", &a::BIC, &a::m1_LLI},  {"BIC", &a::BIC, &a::m1_LLR},  {"BIC", &a::BIC, &a::m1_LRI},  {"BIC", &a::BIC, &a::m1_LRR},  {"BIC", &a::BIC, &a::m1_ARI},  {"BIC", &a::BIC, &a::m1_ARR},  {"BIC", &a::BIC, &a::m1_RRI},  {"BIC", &a::BIC, &a::m1_RRR},  {"BIC", &a::BIC, &a::m1_LLI},  {"???", &a::NOP, &a::XXX},      {"BIC", &a::BIC, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_IMO}, {"BIC", &a::BIC, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"BIC", &a::BIC, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"BICS", &a::BIC, &a::m1_LLI}, {"BICS", &a::BIC, &a::m1_LLR}, {"BICS", &a::BIC, &a::m1_LRI}, {"BICS", &a::BIC, &a::m1_LRR}, {"BICS", &a::BIC, &a::m1_ARI}, {"BICS", &a::BIC, &a::m1_ARR}, {"BICS", &a::BIC, &a::m1_RRI}, {"BICS", &a::BIC, &a::m1_RRR}, {"BICS", &a::BIC, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"BICS", &a::BIC, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_IMO}, {"BICS", &a::BIC, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_IMO}, {"BICS", &a::BIC, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_IMO}},
		{{"MVN", &a::MVN, &a::m1_LLI},  {"MVN", &a::MVN, &a::m1_LLR},  {"MVN", &a::MVN, &a::m1_LRI},  {"MVN", &a::MVN, &a::m1_LRR},  {"MVN", &a::MVN, &a::m1_ARI},  {"MVN", &a::MVN, &a::m1_ARR},  {"MVN", &a::MVN, &a::m1_RRI},  {"MVN", &a::MVN, &a::m1_RRR},  {"MVN", &a::MVN, &a::m1_LLI},  {"???", &a::NOP, &a::XXX},      {"MVN", &a::MVN, &a::m1_LRI},  {"STRH", &a::STRH, &a::m3_PIM}, {"MVN", &a::MVN, &a::m1_ARI},  {"???", &a::NOP, &a::XXX},        {"MVN", &a::MVN, &a::m1_RRI},  {"???", &a::NOP, &a::XXX}},
		{{"MVNS", &a::MVN, &a::m1_LLI}, {"MVNS", &a::MVN, &a::m1_LLR}, {"MVNS", &a::MVN, &a::m1_LRI}, {"MVNS", &a::MVN, &a::m1_LRR}, {"MVNS", &a::MVN, &a::m1_ARI}, {"MVNS", &a::MVN, &a::m1_ARR}, {"MVNS", &a::MVN, &a::m1_RRI}, {"MVNS", &a::MVN, &a::m1_RRR}, {"MVNS", &a::MVN, &a::m1_LLI}, {"???", &a::NOP, &a::XXX},      {"MVNS", &a::MVN, &a::m1_LRI}, {"LDRH", &a::LDRH, &a::m3_PIM}, {"MVNS", &a::MVN, &a::m1_ARI}, {"LDRSB", &a::LDRSB, &a::m3_PIM}, {"MVNS", &a::MVN, &a::m1_RRI}, {"LDRSH", &a::LDRSH, &a::m3_PIM}},
		
		{{"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},   {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},   {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM},     {"AND", &a::AND, &a::m1_IMM},  {"AND", &a::AND, &a::m1_IMM}},
		{{"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM},  {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM},  {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM},    {"ANDS", &a::AND, &a::m1_IMM}, {"ANDS", &a::AND, &a::m1_IMM}},
		{{"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},   {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},   {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM},     {"EOR", &a::EOR, &a::m1_IMM},  {"EOR", &a::EOR, &a::m1_IMM}},
		{{"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM},  {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM},  {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM},    {"EORS", &a::EOR, &a::m1_IMM}, {"EORS", &a::EOR, &a::m1_IMM}},
		{{"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},	{"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},   {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM},     {"SUB", &a::SUB, &a::m1_IMM},  {"SUB", &a::SUB, &a::m1_IMM}},
		{{"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM},  {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM},  {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM},    {"SUBS", &a::SUB, &a::m1_IMM}, {"SUBS", &a::SUB, &a::m1_IMM}},
		{{"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},	{"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},   {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM},     {"RSB", &a::RSB, &a::m1_IMM},  {"RSB", &a::RSB, &a::m1_IMM}},
		{{"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM},  {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM},  {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM},    {"RSBS", &a::RSB, &a::m1_IMM}, {"RSBS", &a::RSB, &a::m1_IMM}},
		{{"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},	{"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},   {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM},     {"ADD", &a::ADD, &a::m1_IMM},  {"ADD", &a::ADD, &a::m1_IMM}},
		{{"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM},  {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM},  {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM},    {"ADDS", &a::ADD, &a::m1_IMM}, {"ADDS", &a::ADD, &a::m1_IMM}},
		{{"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},	{"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},   {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM},     {"ADC", &a::ADC, &a::m1_IMM},  {"ADC", &a::ADC, &a::m1_IMM}},
		{{"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM},  {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM},  {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM},    {"ADCS", &a::ADC, &a::m1_IMM}, {"ADCS", &a::ADC, &a::m1_IMM}},
		{{"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},	{"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},   {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM},     {"SBC", &a::SBC, &a::m1_IMM},  {"SBC", &a::SBC, &a::m1_IMM}},
		{{"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM},  {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM},  {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM},    {"SBCS", &a::SBC, &a::m1_IMM}, {"SBCS", &a::SBC, &a::m1_IMM}},
		{{"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},	{"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},   {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM},     {"RSC", &a::RSC, &a::m1_IMM},  {"RSC", &a::RSC, &a::m1_IMM}},
		{{"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM},  {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM},  {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM},    {"RSCS", &a::RSC, &a::m1_IMM}, {"RSCS", &a::RSC, &a::m1_IMM}},
		{{"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},      {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},      {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},        {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX}},
		{{"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM},  {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM},  {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM},    {"TSTS", &a::TST, &a::m1_IMM}, {"TSTS", &a::TST, &a::m1_IMM}},
		{{"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},   {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},   {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},     {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM}},
		{{"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM},  {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM},  {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM},    {"TEQS", &a::TEQ, &a::m1_IMM}, {"TEQS", &a::TEQ, &a::m1_IMM}},
		{{"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},      {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},      {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX},        {"???", &a::NOP, &a::XXX},     {"???", &a::NOP, &a::XXX}},
		{{"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM},  {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM},  {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM},    {"CMPS", &a::CMP, &a::m1_IMM}, {"CMPS", &a::CMP, &a::m1_IMM}},
		{{"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},   {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},   {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM},     {"MSR", &a::MSR, &a::m1_IMM},  {"MSR", &a::MSR, &a::m1_IMM}},
		{{"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM},  {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM},  {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM},    {"CMNS", &a::CMN, &a::m1_IMM}, {"CMNS", &a::CMN, &a::m1_IMM}},
		{{"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},   {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},   {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM},     {"ORR", &a::ORR, &a::m1_IMM},  {"ORR", &a::ORR, &a::m1_IMM}},
		{{"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM},  {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM},  {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM},    {"ORRS", &a::ORR, &a::m1_IMM}, {"ORRS", &a::ORR, &a::m1_IMM}},
		{{"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},   {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},   {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM},     {"MOV", &a::MOV, &a::m1_IMM},  {"MOV", &a::MOV, &a::m1_IMM}},
		{{"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM},  {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM},  {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM},    {"MOVS", &a::MOV, &a::m1_IMM}, {"MOVS", &a::MOV, &a::m1_IMM}},
		{{"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},   {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},   {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM},     {"BIC", &a::BIC, &a::m1_IMM},  {"BIC", &a::BIC, &a::m1_IMM}},
		{{"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM},  {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM},  {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM},    {"BICS", &a::BIC, &a::m1_IMM}, {"BICS", &a::BIC, &a::m1_IMM}},
		{{"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},   {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},   {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM},     {"MVN", &a::MVN, &a::m1_IMM},  {"MVN", &a::MVN, &a::m1_IMM}},
		{{"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM},  {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM},  {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM},    {"MVNS", &a::MVN, &a::m1_IMM}, {"MVNS", &a::MVN, &a::m1_IMM}},
		{{"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},   {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},   {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},     {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP}},
		{{"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},   {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},   {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},     {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP}},
		{{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP}, {"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP}, {"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},   {"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP}},
		{{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP}, {"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP}, {"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},   {"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP}},
		{{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP}, {"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP}, {"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},   {"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP}},
		{{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP}, {"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP}, {"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},   {"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP}},
		{{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP}, {"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP}, {"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},   {"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP}},
		{{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP}, {"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP}, {"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},   {"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP}},
		{{"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},   {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},   {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP},     {"STR", &a::STR, &a::m2_IMP},  {"STR", &a::STR, &a::m2_IMP}},
		{{"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},   {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},   {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP},     {"LDR", &a::LDR, &a::m2_IMP},  {"LDR", &a::LDR, &a::m2_IMP}},
		{{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP}, {"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP}, {"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP},   {"STRT", &a::STRT, &a::m2_IMP},{"STRT", &a::STRT, &a::m2_IMP}},
		{{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP}, {"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP}, {"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP},   {"LDRT", &a::LDRT, &a::m2_IMP},{"LDRT", &a::LDRT, &a::m2_IMP}},
		{{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP}, {"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP}, {"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP},   {"STRB", &a::STRB, &a::m2_IMP},{"STRB", &a::STRB, &a::m2_IMP}},
		{{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP}, {"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP}, {"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP},   {"LDRB", &a::LDRB, &a::m2_IMP},{"LDRB", &a::LDRB, &a::m2_IMP}},
		{{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP}, {"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP}, {"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP},   {"STRBT",&a::STRBT,&a::m2_IMP},{"STRBT",&a::STRBT,&a::m2_IMP}},
		{{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP}, {"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP}, {"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP},   {"LDRBT",&a::LDRBT,&a::m2_IMP},{"LDRBT",&a::LDRBT,&a::m2_IMP}},
		{{"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},   {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},   {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},     {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO}},
		{{"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},   {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},   {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},     {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO}},
		{{"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},   {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},   {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},     {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM}},
		{{"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},   {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},   {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},     {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM}},
		{{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO}, {"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO}, {"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},   {"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO}},
		{{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO}, {"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO}, {"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},   {"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO}},
		{{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM}, {"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM}, {"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},   {"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM}},
		{{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM}, {"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM}, {"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},   {"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM}},
		{{"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},   {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},   {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO},     {"STR", &a::STR, &a::m2_IMO},  {"STR", &a::STR, &a::m2_IMO}},
		{{"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},   {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},   {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO},     {"LDR", &a::LDR, &a::m2_IMO},  {"LDR", &a::LDR, &a::m2_IMO}},
		{{"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},   {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},   {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM},     {"STR", &a::STR, &a::m2_PIM},  {"STR", &a::STR, &a::m2_PIM}},
		{{"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},   {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},   {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM},     {"LDR", &a::LDR, &a::m2_PIM},  {"LDR", &a::LDR, &a::m2_PIM}},
		{{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO}, {"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO}, {"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO},   {"STRB", &a::STRB, &a::m2_IMO},{"STRB", &a::STRB, &a::m2_IMO}},
		{{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO}, {"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO}, {"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO},   {"LDRB", &a::LDRB, &a::m2_IMO},{"LDRB", &a::LDRB, &a::m2_IMO}},
		{{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM}, {"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM}, {"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM},   {"STRB", &a::STRB, &a::m2_PIM},{"STRB", &a::STRB, &a::m2_PIM}},
		{{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM}, {"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM}, {"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM},   {"LDRB", &a::LDRB, &a::m2_PIM},{"LDRB", &a::LDRB, &a::m2_PIM}},
		//60
		{{"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},   {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},    {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX}},
		{{"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},   {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},    {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX}},
		{{"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STRT", &a::STRT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},   {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},    {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX}},
		{{"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDRT", &a::LDRT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},   {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},    {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX}},
		{{"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},   {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},    {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX}},
		{{"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},   {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},    {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX}},
		{{"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},   {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},   {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},    {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},      {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},      {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX}},
		{{"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},   {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},   {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},    {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},      {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},      {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX}},
		{{"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},   {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},    {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX}},
		{{"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},   {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},    {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_SRP},    {"???", &a::NOP, &a::XXX}},
		{{"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STRT", &a::STRT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},   {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},    {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"STRT", &a::STRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX}},
		{{"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDRT", &a::LDRT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},   {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},    {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"LDRT", &a::LDRT, &a::m2_SRP},  {"???", &a::NOP, &a::XXX}},
		{{"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},   {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},    {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX}},
		{{"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},   {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},    {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX},      {"LDRB", &a::LDRB, &a::m2_SRP},  {"???", &a::NOP, &a::XXX}},
		{{"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},   {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},   {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},    {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},      {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},      {"STRBT", &a::STRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX}},
		{{"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},     {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},   {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},   {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},    {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},      {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX},      {"LDRBT", &a::LDRBT, &a::m2_SRP},{"???", &a::NOP, &a::XXX}},
		{{"STR", &a::STR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},   {"STR", &a::STR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},    {"STR", &a::STR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX}},
		{{"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},   {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},    {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX}},
		{{"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},   {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},    {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX}},
		{{"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},   {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},    {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX}},
		{{"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRO},{"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},   {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},    {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX}},
		{{"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRO},{"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},   {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},    {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},      {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX}},
		{{"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},   {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},    {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_SRO},    {"???", &a::NOP, &a::XXX}},
		{{"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},     {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},   {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},    {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},      {"STR", &a::STR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX}},
		{{"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},     {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},   {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},    {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX},      {"LDR", &a::LDR, &a::m2_PSR},    {"???", &a::NOP, &a::XXX}},
		{{"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRO},{"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},   {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},    {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX}},
		{{"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRO},{"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},   {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},    {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},      {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX},      {"LDRB", &a::LDRB, &a::m2_SRO},  {"???", &a::NOP, &a::XXX}},
		{{"STRB", &a::STRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_PSR},{"???", &a::NOP, &a::XXX},     {"STRB", &a::STRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},   {"STRB", &a::STRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},    {"STRB", &a::STRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},      {"STRB", &a::STRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX}},
		{{"LDRB", &a::LDRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_PSR},{"???", &a::NOP, &a::XXX},     {"LDRB", &a::LDRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},   {"LDRB", &a::LDRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},    {"LDRB", &a::LDRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},      {"LDRB", &a::LDRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX},      {"LDRB", &a::LDRB, &a::m2_PSR},  {"???", &a::NOP, &a::XXX}},
		//80
		{{"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}},
		{{"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}},
		{{"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}, {"STMDA", &a::STM_1, &a::m4_DA}},
		{{"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}, {"LDMDA", &a::LDM_1, &a::m4_DA}},
		{{"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}},
		{{"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}},
		{{"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}, {"STMDA", &a::STM_2, &a::m4_DA}},
		{{"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}, {"LDMDA", &a::LDM_2, &a::m4_DA}},
		{{"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}},
		{{"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}},
		{{"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}, {"STMIA", &a::STM_1, &a::m4_IA}},
		{{"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}, {"LDMIA", &a::LDM_1, &a::m4_IA}},
		{{"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}},
		{{"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}},
		{{"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}, {"STMIA", &a::STM_2, &a::m4_IA}},
		{{"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}, {"LDMIA", &a::LDM_2, &a::m4_IA}},
		{{"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}},
		{{"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}},
		{{"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}, {"STMDB", &a::STM_1, &a::m4_DB}},
		{{"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}, {"LDMDB", &a::LDM_1, &a::m4_DB}},
		{{"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}},
		{{"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}},
		{{"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}, {"STMDB", &a::STM_2, &a::m4_DB}},
		{{"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}, {"LDMDB", &a::LDM_2, &a::m4_DB}},
		{{"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}},
		{{"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}},
		{{"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}, {"STMIB", &a::STM_1, &a::m4_IB}},
		{{"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}, {"LDMIB", &a::LDM_1, &a::m4_IB}},
		{{"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}},
		{{"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}},
		{{"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}, {"STMIB", &a::STM_2, &a::m4_IB}},
		{{"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}, {"LDMIB", &a::LDM_2, &a::m4_IB}},
		//A0
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		{{"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}, {"B", &a::B, &a::XXX}},
		//B0
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		{{"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}, {"BL", &a::BL, &a::XXX}},
		//BF
		//Add Multiprocessor Instructions later if needed
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		{{"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}, {"???", &a::NOP, &a::XXX}},
		//F0
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
		{{"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}, {"SWI", &a::SWI, &a::XXX}},
	};
	
}

ARM7TDMI::~ARM7TDMI() {

}

//register helpers
uint32_t ARM7TDMI::readRegister(uint8_t n) {
	if (n <= 12) {
		return r[n];
	}
	else if (n == 13) {
		return sp;
	}
	else if (n == 14) {
		return lr;
	}
	else if (n == 15) {
		return pc;
	}
}

void ARM7TDMI::writeRegister(uint8_t n, uint32_t data) {
	if (n <= 12) {
		r[n] = data;
	}
	else if (n == 13) {
		sp = data;
	}
	else if (n == 14) {
		lr = data;
	}
	else if (n == 15) {
		pc = data;
	}
}

//condition code
bool ARM7TDMI::conditionPassed(uint32_t condition) {
	return cond_lookup[condition].cond;
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
uint32_t ARM7TDMI::XXX() {
	return 0;
}

//Mode 1
uint32_t ARM7TDMI::m1_IMM() {
	uint32_t rotate_imm = (opcode >> 8) & 0xf;
	uint32_t immed_8 = (opcode & 0xff);
	if (rotate_imm == 0) {
		shifter_carry_out = cpsr.C;
	}
	else {
		shifter_operand = rotateRight(immed_8, rotate_imm * 2);
		shifter_carry_out = shifter_operand >> 31;
	}
	return 0;
}

uint32_t ARM7TDMI::m1_REG() {
	shifter_operand = readRegister(opcode & 0b1111);
	shifter_carry_out = cpsr.C;
	return 0;
}

uint32_t ARM7TDMI::m1_LLI() {
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

uint32_t ARM7TDMI::m1_LLR() {
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
	return 0;
}

uint32_t ARM7TDMI::m1_LRI() {
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

uint32_t ARM7TDMI::m1_LRR() {
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
	return 0;
}

uint32_t ARM7TDMI::m1_ARI() {
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

uint32_t ARM7TDMI::m1_ARR() {
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
	return 0;
}

uint32_t ARM7TDMI::m1_RRI() {
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

uint32_t ARM7TDMI::m1_RRR() {
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

uint32_t ARM7TDMI::m1_RRX() {
	uint32_t Rm = readRegister(opcode & 0xf);
	shifter_operand = (Rm >> 1) | (cpsr.C << 31);
	shifter_carry_out = Rm & 0x1;
	return 0;
}

//Mode 2
uint32_t ARM7TDMI::m2_IMO() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t offset_12 = opcode & 0xfff;
	if (((opcode >> 23) & 0x1) == 1) {
		addr = Rn + offset_12;
	}
	else {
		addr = Rn - offset_12;
	}
	return 0;
}
uint32_t ARM7TDMI::m2_RGO() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	if (((opcode >> 23) & 0x1) == 1) {
		addr = Rn + Rm;
	}
	else {
		addr = Rn - Rm;
	}
	return 0;
}
uint32_t ARM7TDMI::m2_SRO() {
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
		addr = Rn + index;
	}
	else {
		addr = Rn - index;
	}
	return 0;
}
uint32_t ARM7TDMI::m2_PIM() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t offset_12 = opcode & 0xfff;
	if (((opcode >> 23) & 0x1) == 1) {
		addr = Rn + offset_12;
	}
	else {
		addr = Rn - offset_12;
	}
	if (conditionPassed((opcode >> 28) & 0b1111)) {
		writeRegister((opcode >> 16) & 0xf, addr); //write back into Rn
	}
	return 0;
}
uint32_t ARM7TDMI::m2_PRG() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	if (((opcode >> 23) & 0x1) == 1) {
		addr = Rn + Rm;
	}
	else {
		addr = Rn - Rm;
	}
	if (conditionPassed((opcode >> 28) & 0b1111)) {
		writeRegister((opcode >> 16) & 0xf, addr); //write back into Rn
	}
	return 0;
}
uint32_t ARM7TDMI::m2_PSR() {
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
		addr = Rn + index;
	}
	else {
		addr = Rn - index;
	}
	if (conditionPassed((opcode >> 28) & 0b1111)) {
		writeRegister((opcode >> 16) & 0xf, addr); //write back into Rn
	}
	return 0;
}
uint32_t ARM7TDMI::m2_IMP() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	addr = Rn;
	if (conditionPassed((opcode >> 28) & 0b1111)) {
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
uint32_t ARM7TDMI::m2_RGP() {
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	addr = Rn;
	if (conditionPassed((opcode >> 28) & 0b1111)) {
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
uint32_t ARM7TDMI::m2_SRP() {
	uint32_t shift = (opcode >> 5) & 0b11;
	uint32_t shift_imm = (opcode >> 7) & 0b11111;
	uint32_t Rn = readRegister((opcode >> 16) & 0xf);
	uint32_t Rm = readRegister(opcode & 0xf);
	uint32_t index = 0;
	addr = Rn;
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
	
	if (conditionPassed((opcode >> 28) & 0b1111)) {
		if (((opcode >> 23) & 0x1) == 1) {
			writeRegister((opcode >> 16) & 0xf, Rn + index);
		}
		else {
			writeRegister((opcode >> 16) & 0xf, Rn - index);
		}
	}
	return 0;
}

