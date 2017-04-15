#ifndef SIMULATE_H_INCLUDED
#define SIMULATE_H_INCLUDED
#include <bits/stdc++.h>
#include "memory.h"
#include "regFile.h"
#include "pipelineReg.h"
#include "ALU.h"
#include "hazard.h"

using namespace std;

class Simulator{
public:
    Hazard hazard;

    bool branchJr;
    bool stall;
    bool flush;

    bool forward_EX_MEM_rs_ID;
    bool forward_EX_MEM_rt_ID;
    bool forward_MEM_WB_rs_ID;
    bool forward_MEM_WB_rt_ID;
    bool forward_EX_MEM_rs_EX;
    bool forward_EX_MEM_rt_EX;
    bool forward_MEM_WB_rs_EX;
    bool forward_MEM_WB_rt_EX;

    void instructionFetch();
    void instructionDecode();
    void executeOp();
    void memoryAccess();
    void writeBack();

    void updatePipelineReg();
};




#endif // SIMULATE_H_INCLUDED

