#include <bits/stdc++.h>
#include "pipelineReg.h"
using namespace std;

///PipelineIF_ID///
PipelineIF_ID::PipelineIF_ID(){
    inst = 0;
}
///PipelineID_EX///
PipelineID_EX::PipelineID_EX(){
    rsData = rtData = 0;
    signExtendImme = unsignExtendImme = 0;
}
///PipelineEX_MEM///
PipelineEX_MEM::PipelineEX_MEM(){
    rsData = rtData = 0;
    signExtendImme = unsignExtendImme = 0;
    ALUOut = ALUOut64 = 0;
    HI = LO = 0;
}
///pipelineMEM_WB///
PipelineMEM_WB::PipelineMEM_WB(){
    rtData = 0;
    ALUOut = 0;
    HI = LO = 0;
    writeBackData = 0;
}
///PipelineWB///
PipelineWB::PipelineWB(){
    HI = LO = 0;
    writeBackData = 0;
}

