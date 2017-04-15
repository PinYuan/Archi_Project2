#include <bits/stdc++.h>
#include "piperpt.h"
#include "pipelineReg.h"
using namespace std;

extern PipelineIF_ID pipelineIF_IDIn;
extern PipelineID_EX pipelineID_EXIn;
extern PipelineEX_MEM pipelineEX_MEMIn;
extern PipelineMEM_WB pipelineMEM_WBIn;
extern PipelineWB pipelineWB;

void hazardReport(bool flush, bool stall, bool forward_EX_MEM_rs_ID ,bool forward_MEM_WB_rs_ID, bool forward_EX_MEM_rt_ID,
                  bool forward_MEM_WB_rt_ID, bool forward_EX_MEM_rs_EX, bool forward_MEM_WB_rs_EX, bool forward_EX_MEM_rt_EX,
                  bool forward_MEM_WB_rt_EX){
    FILE* fptr;
    fptr = fopen("snapshot.rpt", "a");
    if(!fptr){printf("open file error\n");return;}

    ///IF///
    fprintf(fptr, "IF: 0x%08X", pipelineIF_IDIn.inst);
    if(stall) fprintf(fptr, " to_be_stalled");
    else if(flush) fprintf(fptr, " to_be_flushed");
    fprintf(fptr, "\n");
    ///ID///
    fprintf(fptr, "ID: %s", pipelineID_EXIn.inst.name.c_str());
    if(stall) fprintf(fptr, " to_be_stalled");
    else{
        if(forward_EX_MEM_rs_ID) fprintf(fptr, " fwd_EX-DM_rs_$%d", pipelineID_EXIn.inst.regRs);
        if(forward_MEM_WB_rs_ID) fprintf(fptr, " fwd_DM-WB_rs_$%d", pipelineID_EXIn.inst.regRs);
        if(forward_EX_MEM_rt_ID) fprintf(fptr, " fwd_EX-DM_rt_$%d", pipelineID_EXIn.inst.regRt);
        if(forward_MEM_WB_rt_ID) fprintf(fptr, " fwd_DM-WB_rt_$%d", pipelineID_EXIn.inst.regRt);
    }
    //branch forward
    fprintf(fptr, "\n");
    ///EX///
    fprintf(fptr, "EX: %s", pipelineEX_MEMIn.inst.name.c_str());
    if(forward_EX_MEM_rs_EX) fprintf(fptr, " fwd_EX-DM_rs_$%d", pipelineEX_MEMIn.inst.regRs);
    if(forward_MEM_WB_rs_EX) fprintf(fptr, " fwd_DM-WB_rs_$%d", pipelineEX_MEMIn.inst.regRs);
    if(forward_EX_MEM_rt_EX) fprintf(fptr, " fwd_EX-DM_rt_$%d", pipelineEX_MEMIn.inst.regRt);
    if(forward_MEM_WB_rt_EX) fprintf(fptr, " fwd_DM-WB_rt_$%d", pipelineEX_MEMIn.inst.regRt);
    fprintf(fptr, "\n");
    ///DM///
    fprintf(fptr, "DM: %s", pipelineMEM_WBIn.inst.name.c_str());
    fprintf(fptr, "\n");
    ///WB///
    fprintf(fptr, "WB: %s", pipelineWB.inst.name.c_str());
    fprintf(fptr, "\n");


    fprintf(fptr, "\n\n");
    fclose(fptr);
}

