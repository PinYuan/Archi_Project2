#ifndef PIPELINEREG_H_INCLUDED
#define PIPELINEREG_H_INCLUDED
#include "instruction.h"
class PipelineIF_ID{
public:
    unsigned int inst;
    PipelineIF_ID();
};

class PipelineID_EX{
public:
    Instruction inst;
    unsigned int rsData, rtData;
    unsigned int signExtendImme;
    unsigned int unsignExtendImme;
    PipelineID_EX();
};

class PipelineEX_MEM{
public:
    Instruction inst;
    unsigned int rsData;
    unsigned int rtData;
    unsigned int signExtendImme;
    unsigned int unsignExtendImme;
    unsigned int ALUOut;
    unsigned long long int ALUOut64;
    unsigned int HI;
    unsigned int LO;
    PipelineEX_MEM();
};

class PipelineMEM_WB{
public:
    Instruction inst;
    unsigned int rtData;
    unsigned int ALUOut;
    unsigned int HI;
    unsigned int LO;
    unsigned int writeBackData;
    PipelineMEM_WB();
};

class PipelineWB{
public:
    Instruction inst;
    unsigned int HI;
    unsigned int LO;
    unsigned int writeBackData;
    PipelineWB();
};

#endif // PIPELINEREG_H_INCLUDED

