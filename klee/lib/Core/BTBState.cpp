//===-- BTBState.cpp -----------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "klee/BTBState.h"
#include "BTBUtil.h"
#include "klee/Internal/Support/ErrorHandling.h"

#include "klee/Internal/Module/InstructionInfoTable.h"
#include "klee/Internal/Module/KInstruction.h"
#include "klee/Internal/Module/KModule.h"
#include "klee/Internal/Module/KInstIterator.h"

#include "llvm/Support/ErrorHandling.h"

#include <iostream> 
#include <time.h>
#include <climits>

using namespace llvm;
using namespace klee;

//constructor
BTBSet::BTBSet(uint8_t ways){
	//llvm::errs() << "Create cnt: ";
	for (uint8_t i = 0; i < ways; i++){
		cnt.push_back(ways-1);
		tagBTB.push_back(0);
		BTB.push_back(nullptr);
		function.push_back(nullptr);
		inst.push_back("");
		functionName.push_back("");
		assemblyLine.push_back(0);
		numArgs.push_back(UINT_MAX);
		functionReturnType.push_back(nullptr);
	}
}

//deep copy constructor
BTBSet::BTBSet(uint8_t ways, const BTBSet &s){
	for (uint8_t i = 0; i < ways; i++){
		cnt.push_back(s.cnt[i]);
		tagBTB.push_back(s.tagBTB[i]);
		BTB.push_back(s.BTB[i]);
		function.push_back(s.function[i]);
		inst.push_back(s.inst[i]);
		functionName.push_back(s.functionName[i]);
		assemblyLine.push_back(s.assemblyLine[i]);
		numArgs.push_back(s.numArgs[i]);
		functionReturnType.push_back(s.functionReturnType[i]);
	}
	// llvm::errs() << "Pass cnt: ";
	// for (int i = 0; i < ways; i++)
	// {
	// 	llvm::errs() << (unsigned)cnt[i] << "\t";
	// }
	// llvm::errs() << "\n";
}

//destructor
BTBSet::~BTBSet() {}

void BTBSet::upCnt(int threshold, uint8_t way){
	for (uint8_t i = 0; i < way; i++)
	{
		if(cnt[i] < threshold) cnt[i]++;
	}
}

void BTBSet::upCnt(uint8_t way){
	for (uint8_t i = 0; i < way; i++)
	{
		if(cnt[i] < way-1) cnt[i]++;
	}
}

bool BTBSet::isFull(uint8_t way){
	for (uint8_t i = 0; i < way; i++)
	{
		if(BTB[i] == nullptr){
			return false;
		}
	}
	return true;
}

void BTBSet::dump(uint8_t way){
	llvm::errs() << "Dump cnt( " << (unsigned)way << "): ";
	for (uint8_t i = 0; i < way; i++)
	{
		llvm::errs() << (unsigned)cnt[i] << "\t";
	}
	llvm::errs() << "\n";
}

// The drawn flowchart
KInstIterator BTBSet::getPC(uint64_t tag, uint8_t way, KFunction* kf, std::string source){
	//llvm::errs() << "read cnt( " << (unsigned)way << ")\n";
	//klee_message("------------get PC from BTB------------");
	for (uint8_t i = 0; i < way; i++)
	{
		if((tagBTB[i] == tag) && (BTB[i] != nullptr)){
			//klee_message("------------ Tag is equal and BTB not empty------------");
			if((source == inst[i]) && (source == "BR")){
				//klee_message("------------Source is BR------------");
				if(kf->getFunctionName() == functionName[i]){
					//klee_message("------------Return a valid value------------");
					upCnt(cnt[i], way);
					cnt[i] = 0;
					return BTB[i];
				}
			}
			else if ((source == inst[i]) && (source == "CALL")){
				//klee_message("------------Source is CALL------------");
				if((kf->getNumArgs() == numArgs[i]) && (kf->getReturnType() == functionReturnType[i])){
					//klee_message("------------Return a valid value------------");
					upCnt(cnt[i], way);
					cnt[i] = 0;
					return BTB[i];
				}
				// else{
				// 	if(kf->getNumArgs() != numArgs[i]){
				// 		klee_message("------------Num Args: real(%d), btb(%d) ------------", kf->getNumArgs(), numArgs[i]);
				// 	}
				// 	if(kf->getReturnType() != functionReturnType[i]){
				// 		llvm::errs() << "Return type for real function is: ";
				// 		kf->getReturnType()->print(llvm::errs());
				// 		llvm::errs() << "\n";
				// 		llvm::errs() << "Return type for BTB function is: ";
				// 		functionReturnType[i]->print(llvm::errs());
				// 		llvm::errs() << "\n";
				// 	}
				// }
			}
		// 	else{
		// 		klee_message("Instructions are not compatible: r(%s) , btb(%s)", source, inst[i]);
		// 		llvm::errs() << "Instructions are not compatible: r(" << source << ") , btb(" << inst[i] << ") \n";
		// 	}
		 }
	}
	//llvm::errs() << "No compatible TAG\n";
	return nullptr;
}

void BTBSet::update(uint64_t tag, uint8_t way, const KInstIterator &pc, KFunction* kf, std::string instruction){
	//llvm::errs() << "update cnt( " << (unsigned)way << ")\n";
	bool full = isFull(way);
	//klee_message("------------Update BTB------------");
	//check if a corresponding tag exists in set
	for (uint8_t i = 0; i < way; i++)
	{
		if(tagBTB[i] == tag){
			if(BTB[i] != nullptr){
				upCnt(cnt[i], way);
				cnt[i] = 0;
				BTB[i] = pc;
				if(instruction == "BR"){
					//klee_message("------------Update BTB: BR (exist)------------");
					updateBR(i, kf);
				}else if(instruction == "CALL"){
					//klee_message("------------Update BTB: CALL (exist)------------");
					updateCALL(i, kf);
				}else assert(false);
				//llvm::errs() << "Update (exist):" << (unsigned)i << "\n";
				// for (uint8_t i = 0; i < way; i++)
				// {
				// 	llvm::errs() << (unsigned)cnt[i] << "\t";
				// }
				// llvm::errs() << "\n";
				return;
			}
		}
	}
	//it is not in the set
	for (uint8_t i = 0; i < way; i++)
	{
		if(cnt[i] == way-1){
			upCnt(way);
			cnt[i] = 0;
			BTB[i] = pc;
			tagBTB[i] = tag;
			if(instruction == "BR"){
				//klee_message("------------Update BTB: BR (not exist)------------");
				updateBR(i, kf);
			}else if(instruction == "CALL"){
				//klee_message("------------Update BTB: CALL (not exist)------------");
				updateCALL(i, kf);
			}
			/*
			llvm::errs() << "Update (no exist):" << (unsigned)i << "\n";
			for (uint8_t i = 0; i < way; i++)
			{
				llvm::errs() << (unsigned)cnt[i] << "\t";
			}
			llvm::errs() << "\n";
			*/
			return;
		}
	}
	//dump(way);
	//klee_message("CNT=way-1 is not found");
	assert(false);
}

void BTBSet::updateBR(uint8_t id, KFunction* kf){
	setKFunction(id, kf);
	setInst(id, "BR");
	setFuncName(id, kf);
	//llvm::errs() << "Function name for this branch is: " << kf->getFunctionName() << "\n";
	setNumArgs(id, nullptr);
	setReturnType(id, nullptr);
}

void BTBSet::updateCALL(uint8_t id, KFunction* kf){
	setKFunction(id, kf);
	setInst(id, "CALL");
	setFuncName(id, nullptr);
	setNumArgs(id, kf);
	//llvm::errs() << "Number of arguments for this function is: " << kf->getNumArgs() << "\n";
	setReturnType(id, kf);
	//llvm::errs() << "Return type for this function is: ";
	//kf->getReturnType()->print(llvm::errs());
	//llvm::errs() << "\n";
}

std::string BTBSet::getInst(uint64_t tag, uint8_t way){
	//llvm::errs() << "read cnt( " << (unsigned)way << ")\n";
	for (uint8_t i = 0; i < way; i++)
	{
		if((tagBTB[i] == tag) && (inst[i] != "")){
			return inst[i];
		}
	}
	return "";
}

std::string BTBSet::getFuncName(uint64_t tag, uint8_t way){
	//llvm::errs() << "read cnt( " << (unsigned)way << ")\n";
	for (uint8_t i = 0; i < way; i++)
	{
		if((tagBTB[i] == tag) && (functionName[i] != "")){
			return functionName[i];
		}
	}
	return "";
}

KFunction* BTBSet::getKFunction(uint64_t tag, uint8_t way){
	//llvm::errs() << "read cnt( " << (unsigned)way << ")\n";
	for (uint8_t i = 0; i < way; i++)
	{
		if((tagBTB[i] == tag) && (function[i] != nullptr)){
			return function[i];
		}
	}
	return nullptr;
}

unsigned BTBSet::getNumArgs(uint64_t tag, uint8_t way){
	//llvm::errs() << "read cnt( " << (unsigned)way << ")\n";
	for (uint8_t i = 0; i < way; i++)
	{
		if((tagBTB[i] == tag) && (numArgs[i] != UINT_MAX)){
			return numArgs[i];
		}
	}
	return UINT_MAX;
}

llvm::Type* BTBSet::getReturnType(uint64_t tag, uint8_t way){
	//llvm::errs() << "read cnt( " << (unsigned)way << ")\n";
	for (uint8_t i = 0; i < way; i++)
	{
		if((tagBTB[i] == tag) && (functionReturnType[i] != nullptr)){
			return functionReturnType[i];
		}
	}
	return nullptr;
}

void BTBSet::setInst(uint8_t id, std::string instruction){
	inst[id] = instruction;
}

void BTBSet::setFuncName(uint8_t id, KFunction* kf){
	if(kf != nullptr) functionName[id] = kf->getFunctionName();
	else functionName[id] = "";
}

void BTBSet::setKFunction(uint8_t id, KFunction* kf){
	if(kf != nullptr) function[id] = kf;
	else function[id] = nullptr;
}

void BTBSet::setNumArgs(uint8_t id, KFunction* kf){
	if(kf != nullptr) numArgs[id] = kf->getNumArgs();
	else numArgs[id] = UINT_MAX;
}

void BTBSet::setReturnType(uint8_t id, KFunction* kf){
	if(kf != nullptr) functionReturnType[id] = kf->getReturnType();
	else functionReturnType[id] = nullptr;
}

///////////////////////////////////BTB State///////////////////////////////////////
BTBState::BTBState(ExecutionState &_state) : state(_state){
	set_size = BTBConfig::btb_set_size;
	set_bits = BTBUtil::log_base2(set_size);
	tag_bits = BTBConfig::btb_tag_size;
	ways = BTBConfig::btb_ways;

	for (int i = 0; i < set_size; i++){
		btb.push_back(BTBSet(ways));
	}
}

BTBState::BTBState(ExecutionState &_state, const BTBState &cs): state(_state) {
	set_size = BTBConfig::btb_set_size;
	set_bits = BTBUtil::log_base2(set_size);
	tag_bits = BTBConfig::btb_tag_size;
	ways = BTBConfig::btb_ways;

	for (int i = 0; i < set_size; i++){
		btb.push_back(BTBSet(ways, cs.btb[i]));
	}
}

//gets address, dst (e.g., bi->getSuccessor(0)), and program state.
//Computes set and tag bits.
//Based on dst and state, computes PC of the desired next instruction.

KInstIterator BTBState::read(uint64_t addr, KFunction* kf, std::string source){
	uint64_t set_num = BTBUtil::getSet(addr);
	uint64_t tag = 0;
	if(tag_bits > 0){
		tag = BTBUtil::getTag(addr);
	}
	/*
	llvm::errs() << "Read:\n";
	for (int i = 0; i < set_size; i++)
	{
		for (int j = 0; j < ways; j++)
		{
			llvm::errs() << (unsigned)btb[i].cnt[j] << "\t";
		}
		llvm::errs() << "\n";
	}
	*/
	return btb[set_num].getPC(tag, ways, kf, source);
}

KFunction* BTBState::getKFunction(uint64_t addr){
	uint64_t set_num = BTBUtil::getSet(addr);
	uint64_t tag = 0;
	if(tag_bits > 0){
		tag = BTBUtil::getTag(addr);
	}
	/*
	llvm::errs() << "Read:\n";
	for (int i = 0; i < set_size; i++)
	{
		for (int j = 0; j < ways; j++)
		{
			llvm::errs() << (unsigned)btb[i].cnt[j] << "\t";
		}
		llvm::errs() << "\n";
	}
	*/
	return btb[set_num].getKFunction(tag, ways);
}

//storing PC in BTB
void BTBState::update(uint64_t addr, KInstIterator &btbPC, ExecutionState &state, KFunction* kf, std::string source){
    //compute tag and set
	uint64_t set = BTBUtil::getSet(addr);
	uint64_t tag = BTBUtil::getTag(addr);

    //compute pc
    // KFunction *kf = state.stack.back().kf;
    // unsigned entry = kf->basicBlockEntry[dst];
    // KInstIterator pc = &kf->instructions[entry];

	//KInstIterator pc = btbPC;

	btb[set].update(tag, ways, btbPC, kf, source);
	/*
	llvm::errs() << "Update Set: " << set << "\n";
	for (int i = 0; i < set_size; i++)
	{
		for (int j = 0; j < ways; j++)
		{
			llvm::errs() << (unsigned)btb[i].cnt[j] << "\t";
		}
		llvm::errs() << "\n";
	}
	*/
	return;
}
