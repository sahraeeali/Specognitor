#include "klee/Monitor.h"

#include "klee/Internal/Support/ErrorHandling.h"
#include "klee/Internal/Module/KInstruction.h"
#include "TimingSolver.h"
#include "klee/ExecutionState.h"
#include "CacheState.h"

#include<list>
#include<vector>
#include<cstdlib>
#include<memory>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>

#define N 5

using namespace llvm;
using namespace klee;

/*
1- uint8_t _label
2- uint8_t _isSpeculative
3- int _startTTL
4- bool _stopTTL
5- std::string _instruction
6- uint8_t _isSensitive
7- uint8_t _checkCacheState
8- uint8_t _isConst
*/

Json::Value jsonVal::jsonFile = {};

FSM::FSM(){

    //fsmNode(uint8_t _label, uint8_t _isSpeculative, int _startTTL, bool _stopTTL, std::string _instruction, uint8_t _isSensitive, uint8_t _checkCacheState, uint8_t _isConst)
    //BR
    //nodes.push_back(new fsmNode(0, 0, 32, false, "Br", 2, 2, 0));
    //Load
    //nodes.push_back(new fsmNode(1, 1, 0, false, "Load", 2, 2, 0));
    //Load
    //nodes.push_back(new fsmNode(2, 1, 0, true, "Load", 1, 1, 2));

    //Just one sensitive Load.
    //nodes.push_back(new fsmNode(0, 1, 0, false, "Load", 1, 2, 2));

    if(!jsonVal::jsonFile.isNull()){
        for (uint8_t i = 0; i < jsonVal::jsonFile.size() ;i++)
        {
            std::string s = std::to_string(i);
            std::string sttl_string = jsonVal::jsonFile[s]["stopTTL"].asString();
            bool sttl_bool = false;
            if(sttl_string == "true"){
                sttl_bool = true;
            }
            nodes.push_back(new fsmNode(i, (uint8_t) jsonVal::jsonFile[s]["isSpeculative"].asInt(), jsonVal::jsonFile[s]["startTTL"].asInt(), sttl_bool, jsonVal::jsonFile[s]["instruction"].asString(), (uint8_t) jsonVal::jsonFile[s]["isSensitive"].asInt(), (uint8_t) jsonVal::jsonFile[s]["checkCacheState"].asInt(), (uint8_t) jsonVal::jsonFile[s]["isConst"].asInt()));
        }
    }
    

    //nodes.push_back(new fsmNode(0, 1, 0, false, "Load", 1, 1, 2));

    //two loads
    //nodes.push_back(new fsmNode(0, 1, 20, false, "Load", 2, 2, 2));
    //nodes.push_back(new fsmNode(1, 1, 0, true, "Load", 1, 1, 2));

    //Load
    //nodes.push_back(new fsmNode(0, 0, 0, false, "Load", 2, 2, 0));
    //BR
    //nodes.push_back(new fsmNode(1, 0, 0, false, "Br", 2, 2, 0));
    //Load
    //nodes.push_back(new fsmNode(2, 1, 0, true, "Load", 1, 2, 2));
    //std::cout << jsonVal::jsonFile << "\n";
    num_nodes = nodes.size();
    //klee_message("-------------- %d and %d ----------", jsonVal::jsonFile.size(), num_nodes);
    //}
}

//----------------------

Monitor::Monitor(ExecutionState* es) {
    //klee_message("------------------- Create a new empty Monitor ----------------");
    this->fsm = new FSM();
    this->state = es;
    for(int i = 0; i < fsm->num_nodes; i++){
        std::list<std::shared_ptr<iTrace>> l;
        patterns.push_back(l);
    }
    assert(patterns.size() == fsm->num_nodes);
}


Monitor::Monitor(ExecutionState* es, const Monitor &mn){
    //klee_message("------------------- Create a new Monitor ----------------");
    this->fsm = mn.fsm;
    this->state = es;
    this->returnState = mn.returnState;
    //copy patterns
    for(unsigned i = 0; i < mn.patterns.size(); i++){
        //klee_message("------------ old Patterns %d = %ld ----------------", i, mn.patterns[i].size());
        std::list<std::shared_ptr<iTrace>> l; //temp list
        for(std::list<std::shared_ptr<iTrace>>::const_iterator it = mn.patterns[i].begin(); it != mn.patterns[i].end(); it++){
            l.push_back(std::make_shared<iTrace> ((*it)->info, (*it)->ttl, (*it)->pNum));
        }
        patterns.push_back(l);
        //klee_message("------------ new Patterns %d = %ld ----------------", i, patterns[i].size());
    }
    //copy detectedPatterns
    for (std::set<std::vector<std::shared_ptr<iTrace>>>::iterator it = mn.detectedPatterns.begin(); it != mn.detectedPatterns.end(); ++it){
        std::vector<std::shared_ptr<iTrace>> v; //temp list
        for(std::vector<std::shared_ptr<iTrace>>::const_iterator it2 = it->begin(); it2 != it->end(); it2++){
            v.push_back(std::make_shared<iTrace> ((*it2)->info, (*it2)->ttl, (*it2)->pNum));
        }
        detectedPatterns.insert(v);
    }
}

Monitor::~Monitor(){}

uint8_t Monitor::firstEmpty(){
    for (uint8_t i = 0; i < patterns.size(); i++){
        if(patterns[i].size() == 0){
            return i;
        }
    }
    uint8_t x = fsm->num_nodes;
    return x;
}

void Monitor::moveToDetected(const InstructionInfo* i){
    std::vector<std::shared_ptr<iTrace>> shPtr;
    for(uint8_t i = 0; i < patterns.size()-1; i++){
        shPtr.push_back(patterns[i].back());
    }
    shPtr.push_back(std::make_shared<iTrace> (i, -1, 0));
    detectedPatterns.insert(shPtr);
    //print the resulting sequence
    klee_message("----------------------- Leakage Detected --------------------");
    for(std::vector<std::shared_ptr<iTrace>>::iterator j = shPtr.begin(); j != shPtr.end(); j++){
        klee_message("-> %s: %d, ASMLine: %d, liveness: %d", (*j)->info->file.c_str(), (*j)->info->line, (*j)->info->assemblyLine, (*j)->ttl);
    }
    klee_message("////////////////////////////////////////////////////////////////////////////////");
    /*
    printNumPat();
    klee_message("-> %s: %d, ASMLine: %d, liveness: -1", i->file.c_str(), i->line, i->assemblyLine);
    klee_message("////////////////////////////////////////////////////////////////////////////////");
    */
}

/*
struct updateInfo {

    uint8_t isSpec;
    uint8_t isSensitive; //only for address.
    uint8_t isConst;
    uint8_t checkCacheState;
};
*/

bool Monitor::checkEqual(fsmNode* a, struct updateInfo &uInfo){
    if(a->instruction != uInfo.instruction){
        //klee_message("------------ instruction: %ld is not equivalent to node %ld ---------", uInfo.ki->info->assemblyLine, unsigned(a->label) );
        return false;
    }else if((a->isSpeculative < 2) && (a->isSpeculative != uInfo.isSpec)){
        //klee_message("------------ speculation: %ld is not equivalent to node %ld ---------", uInfo.ki->info->assemblyLine, unsigned(a->label) );
        return false;
    }else if((a->isSensitive < 2) && (a->isSensitive != uInfo.isSensitive)){
        //klee_message("------------ sensitive: %ld is not equivalent to node %ld ---------", uInfo.ki->info->assemblyLine, unsigned(a->label) );
        return false;
    }else if((a->isConst < 2) && (a->isConst != uInfo.isConst)){
        //klee_message("------------ const: %ld is not equivalent to node %ld ---------", uInfo.ki->info->assemblyLine, unsigned(a->label) );
        return false;
    }else if((a->checkCacheState < 2) && (a->checkCacheState != uInfo.state.cacheState->verifyCacheSideChannel(uInfo.solver))){
        //klee_message("------------ cache sensitivity: %ld is not equivalent to node %ld ---------", uInfo.ki->info->assemblyLine, unsigned(a->label) );
        return false;
    }
    return true;
}

void Monitor::update(struct updateInfo &uInfo){
    for(std::vector<fsmNode*>::reverse_iterator it = fsm->nodes.rbegin(); it != fsm->nodes.rend(); it++){
        //if the instruction information matches the node information
        if(checkEqual(*it, uInfo)){
            // if it's the first node
            if (it == fsm->nodes.rbegin()){
                //klee_message("------------ Trying to add a last node ---------");
                //check if previous nodes are not empty.
                if(!(firstEmpty() < (*it)->label)){
                    moveToDetected(uInfo.ki->info);
                }
            }else if((*it)->label == 0){
                if((*it)->startTTL > 0){
                    //klee_message("------------ Node 0 added with ttl: %d ---------", (*it)->startTTL);
                    patterns[0].push_front(std::make_shared<iTrace> (uInfo.ki->info, (*it)->startTTL, 0));
                } else {
                    //klee_message("------------ Node 0 added ---------");
                    patterns[0].push_front(std::make_shared<iTrace> (uInfo.ki->info, -1, 0));
                }
                
                //} else {
                    //find the instInfo, update its ttl and push_front
                //}
            //if it's the last node
            }
            //if it's a middle node
            else {
                //klee_message("------------ Trying to add a middle node, First empty: %ld --------- ", unsigned(firstEmpty()));
                if(!(firstEmpty() < (*it)->label)){
                    //klee_message("------------ Middle Node added %d ---------", (*it)->label);
                    std::list<std::shared_ptr<iTrace>> l;
                    if(patterns.size() == (*it)->label){
                        patterns.push_back(l);
                    }
                    //if(/*instInfo is not in patterns[(*it)->label]*/){
                    if((*it)->startTTL > 0){
                        patterns[(*it)->label].push_front(std::make_shared<iTrace> (uInfo.ki->info, (*it)->startTTL, (int)patterns[((*it)->label)-1].size()));
                    } else {
                        patterns[(*it)->label].push_front(std::make_shared<iTrace> (uInfo.ki->info, -1, (int)patterns[((*it)->label)-1].size()));
                    }
                    //} else {
                        //find the instInfo, update its ttl and push_front
                    //}
                }
            }
        }
    }

}

// reduce the parents num.
void Monitor::reduceParent(int index){
    if(index < patterns.size()){
        for(std::list<std::shared_ptr<iTrace>>::iterator it = patterns[index].begin(); it != patterns[index].end(); it++){
            int t = --((*it)->pNum);
            if(t == 0){
                patterns[index].erase(it);
                it--;
                reduceParent(index+1);
            }
        }
    }
}

// remove all patterns from index [i,end]
void Monitor::removePatterns(int index){
    if(index < patterns.size()){
        for(int i = index; i < patterns.size(); i++){
            //klee_message("------------ pattern[%d] is cleared with size %d ---------", i, patterns[i].size());
            patterns[i].clear();
        }
    }
}

void Monitor::step(){
    std::vector<fsmNode*>::iterator n = fsm->nodes.begin();
    for(int i = 0; i < patterns.size(); i++, n++){
        int ttlVal = (*n)->startTTL;
        //klee_message("------------ Patterns %d = %ld ----------------", i, patterns[i].size());
        if(ttlVal > 0){
            std::list<std::shared_ptr<iTrace>>::iterator it = patterns[i].begin();
            int size = patterns[i].size();
            int j = 0;
            while(j < size){
                j++;
                //klee_message("------------ ttl = %d ----------------", (*it)->ttl);
                int t = --((*it)->ttl);
                if (t == 0){
                    patterns[i].erase(it);
                    j++;
                    if(patterns[i].size() == 0){
                        removePatterns(i+1);
                    } else {
                        reduceParent(i+1);
                    }
                }else{
                    it++;
                }
            }

        }
    }
}

void Monitor::printNumPat(){
    for(int i = 0; i < patterns.size(); i++){
        klee_message("------------ Patterns %d = %ld ----------------", i, patterns[i].size());
        for(std::list<std::shared_ptr<iTrace>>::iterator it = patterns[i].begin(); it != patterns[i].end(); it++){
            klee_message("-> %s: %d, ASMLine: %d, liveness: %d", (*it)->info->file.c_str(), (*it)->info->line, (*it)->info->assemblyLine, (*it)->ttl);
        }
    }
}
// Should be fixed. Accumulating monitor is needed instead of replacing.
void Monitor::copyMonitor(){
    if(returnState && returnState->monitor){
        returnState->monitor.reset();
        returnState->monitor = std::make_shared<Monitor> (this->returnState, *this);
        returnState->monitor->state = returnState;
    }   
    returnState->pSpecState = 0;
}

void Monitor::setReturnState(ExecutionState *st){
    this->returnState = st; 
    //klee_message("------------ return state of %ld is %ld ----------------", this->state->tag, st->tag);
}