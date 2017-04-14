#include <iostream>
#include "memory.h"
#include "regFile.h"
#include "ALU.h"
#include "pipelineReg.h"
#include "simulate.h"
#include "instruction.h"

using namespace std;
unsigned int pc, branchJrpc;
unsigned int initialSp;
unsigned int HI;
unsigned int LO;

InstructionMemory IM;
DataMemory DM;
RegisterFile RF;
ALU ALU1;

PipelineIF_ID pipelineIF_IDIn, pipelineIF_IDOut;
PipelineID_EX pipelineID_EXIn, pipelineID_EXOut;
PipelineEX_MEM pipelineEX_MEMIn, pipelineEX_MEMOut;
PipelineMEM_WB pipelineMEM_WBIn, pipelineMEM_WBOut;
PipelineWB pipelineWB;

int main()
{
    Simulator simulator;
    int cycle = 0;
    int halt = 0;
    while(1){
        printf("cycle : %d\n", cycle);
        RF.outputRegister(cycle);

        simulator.writeBack();
        simulator.memoryAccess();
        simulator.executeOp();
        simulator.instructionDecode();
        simulator.instructionFetch();

        if(pipelineWB.inst.name == "HALT")
            halt = 1;


        simulator.hazardReport();
        simulator.updatePipelineReg();

        cycle ++;
        if(simulator.stall) pc = pc;
        else if(simulator.branchJr) pc = branchJrpc;
        else pc += 4;
        if(halt) break;
    }
}

