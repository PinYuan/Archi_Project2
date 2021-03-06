#include <iostream>
#include "memory.h"
#include "regFile.h"
#include "ALU.h"
#include "pipelineReg.h"
#include "simulate.h"
#include "instruction.h"
#include "errorDetect.h"
#include "piperpt.h"

using namespace std;
unsigned int pc, branchJrpc;
unsigned int initialSp;
unsigned int HI;
unsigned int LO;
unsigned int halt;
unsigned int illegal;

InstructionMemory IM;
DataMemory DM;
RegisterFile RF;
ALU ALU1;
ErrorDetect ED;

PipelineIF_ID pipelineIF_IDIn, pipelineIF_IDOut;
PipelineID_EX pipelineID_EXIn, pipelineID_EXOut;
PipelineEX_MEM pipelineEX_MEMIn, pipelineEX_MEMOut;
PipelineMEM_WB pipelineMEM_WBIn, pipelineMEM_WBOut;
PipelineWB pipelineWB;

int main()
{
	ios::sync_with_stdio(false);
    cin.tie(0);

    Simulator simulator;
    int cycle = 0;
    halt = 0;
	illegal = 0;
    while(1){
        RF.outputRegister(cycle);
		ED.setCycle(cycle);

        simulator.writeBack();
        simulator.memoryAccess();
        simulator.executeOp();
        simulator.instructionDecode();
        simulator.instructionFetch();

        if(pipelineWB.inst.name == "HALT")
            halt = 1;
		if(illegal)
			break;

		hazardReport(simulator.flush, simulator.stall, simulator.forward_EX_MEM_rs_ID,
                     simulator.forward_MEM_WB_rs_ID, simulator.forward_EX_MEM_rt_ID,
                     simulator.forward_MEM_WB_rt_ID, simulator.forward_EX_MEM_rs_EX,
                     simulator.forward_MEM_WB_rs_EX, simulator.forward_EX_MEM_rt_EX,
                     simulator.forward_MEM_WB_rt_EX);
        simulator.updatePipelineReg();

        cycle ++;
        if(simulator.stall) pc = pc;
        else if(simulator.branchJr) pc = branchJrpc;
        else pc += 4;
        if(halt) break;
    }
}

