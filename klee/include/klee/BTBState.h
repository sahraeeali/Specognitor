//===-- BTBState.h ------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//
#ifndef KLEE_BTBSTATE_H
#define KLEE_BTBSTATE_H


#include "ExecutionState.h"
#include "../../lib/Core/BTBUtil.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Type.h"

#include "klee/Internal/Module/InstructionInfoTable.h"
#include "klee/Internal/Module/KInstruction.h"
#include "klee/Internal/Module/KModule.h"
#include "klee/Internal/Module/KInstIterator.h"

#include <vector> 
#include <set>
#include <map>
#include <unordered_map> 

namespace llvm {
  class BasicBlock;
  class Type;
  class Function;
}

namespace klee {
    class InstructionInfoTable;
    struct KFunction;
    struct KInstruction;
    class KInstIterator;
    class KModule;

struct BTBSet {
    //counter for unsigned.
    std::vector<uint8_t> cnt;
    // tag bits
    std::vector<uint64_t> tagBTB;
    //PC
    std::vector<KInstIterator> BTB;
    
    //KFunction, CALL: stackFrame.
    std::vector<KFunction*> function; //default: nullptr
    //instruction type: "BR" or "CALL"
    std::vector<std::string> inst; //default: ""
    //BR: name of the function in which BR instruction resides. KFunction->function->getName()
    std::vector<std::string> functionName; //default: ""
    //BR: line number of the target. 
    std::vector<unsigned> assemblyLine; //default: 0
    //CALL: number of arguments for the function.
    std::vector<unsigned> numArgs; //default: UINT_MAX
    //CALL: returnType of a function (Types are identical if pointers are identical [https://llvm.org/docs/ProgrammersManual.html#type])
    std::vector<llvm::Type*> functionReturnType; //default: nullptr

    //dummy constructor
    BTBSet() {}
    //constructor
    BTBSet(uint8_t ways);
    //deep copy constructor
    BTBSet(uint8_t ways, const BTBSet &s);
    //destructor
    ~BTBSet();
    // get tag and return KInstIterator
    KInstIterator getPC(uint64_t tag, uint8_t way, KFunction* kf, std::string source);
    //update PC
    void update(uint64_t tag, uint8_t way, const KInstIterator &pc, KFunction* kf, std::string instruction);
    void updateBR(uint8_t id, KFunction* kf); //updates BR specific items and sets CALL specific items to null/default.
    void updateCALL(uint8_t id, KFunction* kf); //updates CALL specific items and sets BR specific items to null/default.
    //get the source inst type(i.e., the instruction that inserted the entry in BTB).
    std::string getInst(uint64_t tag, uint8_t way);
    void setInst(uint8_t id, std::string instruction);
    //getter and setters for BR
    std::string getFuncName(uint64_t tag, uint8_t way);
    void setFuncName(uint8_t id, KFunction* kf);
    //getter and setters for CALL
    KFunction* getKFunction(uint64_t tag, uint8_t way);
    void setKFunction(uint8_t id, KFunction* kf);
    unsigned getNumArgs(uint64_t tag, uint8_t way);     //unknown is UINT_MAX
    void setNumArgs(uint8_t id, KFunction* kf);
    llvm::Type* getReturnType(uint64_t tag, uint8_t way);
    void setReturnType(uint8_t id, KFunction* kf);

private:
    //update cnt if it's the last element
    void upCnt(uint8_t way);
    //update cnt if it's not the last element
    void upCnt(int index, uint8_t way);
    //returns true if all ways are full
    bool isFull(uint8_t way);
    //dump array
    void dump(uint8_t way);
};

class BTBState {

public:
    //Storing PC in BTB
    // KInstIterator *BTB;
    // uint64_t *tagBTB;
    std::vector<BTBSet> btb;
    //BTBSet* btb;

private:
	// pair of tag and labels
	//std::vector<std::pair <long int, std::string>> cState;
	// mapping of address and label.
    ExecutionState &state;
    uint64_t set_size;
	uint64_t set_bits;
	uint64_t tag_bits;
    uint8_t ways;

public:
	//cacheState(uint64_t set_size, uint64_t set_bits, uint64_t tag_bits);
    // set size is in utils.h -- set bits with log2, tag_bits is more difficult to calculate
    BTBState(ExecutionState &state);
    BTBState(ExecutionState &state, const BTBState &cs);
	
	KInstIterator read(uint64_t addr, KFunction* kf, std::string source);
    KFunction *getKFunction(uint64_t addr);
	void update(uint64_t addr, KInstIterator &btbPC, ExecutionState &state, KFunction* kf, std::string source);
	//for testing
	//void print_BTB();
};

} // end namespace klee

#endif
