#include <bits/stdc++.h>
#include "hazard.h"
#include "simulate.h"
#define ADD 0x20
#define ADDU 0x21
#define SUB 0x22
#define AND 0x24
#define OR 0x25
#define XOR 0x26
#define NOR 0x27
#define NAND 0x28
#define SLT 0x2A
#define SLL 0x00
#define SRL 0x02
#define SRA 0x03
#define JR 0x08
#define MULT 0x18
#define MULTU 0x19
#define MFHI 0x10
#define MFLO 0x12

#define ADDI 0x08
#define ADDIU 0x09
#define LW 0x23
#define LH 0x21
#define LHU 0x25
#define LB 0x20
#define LBU 0x24
#define SW 0x2b
#define SH 0x29
#define SB 0x28
#define LUI 0x0f
#define ANDI 0x0c
#define ORI 0x0d
#define NORI 0x0e
#define SLTI 0x0a
#define BEQ 0x04
#define BNE 0x05
#define BGTZ 0x07

#define J 0x02
#define JAL 0x03
using namespace std;

extern PipelineIF_ID pipelineIF_IDIn, pipelineIF_IDOut;
extern PipelineID_EX pipelineID_EXIn, pipelineID_EXOut;
extern PipelineEX_MEM pipelineEX_MEMIn, pipelineEX_MEMOut;
extern PipelineMEM_WB pipelineMEM_WBIn, pipelineMEM_WBOut;
extern PipelineWB pipelineWB;
/*
stage ID and EX need forward
ID -> branch j
*/
bool Hazard::isEX_MEMForward_rs(unsigned int regRs){
    Instruction inst = pipelineEX_MEMOut.inst;
    bool regWrite = false;
    if(inst.type == 'R'){
        if(inst.func == ADD || inst.func == ADDU || inst.func == SUB || inst.func == AND ||
           inst.func == OR || inst.func == XOR || inst.func == NOR || inst.func == NAND ||
           inst.func == SLT || inst.func == SLL || inst.func == SRL || inst.func == SRA ||
           inst.func == MFHI || inst.func == MFLO){
            regWrite = true;
        }
        if((regWrite == true)&&(inst.regRd != 0)&&(inst.regRd == regRs))
            return true;
    }
    else if(inst.type == 'I'){
        if(inst.opCode == ADDI || inst.opCode == ADDIU || inst.opCode == LUI || inst.opCode == ANDI ||
           inst.opCode == ORI || inst.opCode == NORI || inst.opCode == SLTI){
            regWrite = true;
        }
        if((regWrite == true)&&(inst.regRt != 0)&&(inst.regRt == regRs))
            return true;
    }
    else if(inst.type == 'J'){
        if(inst.opCode == JAL){
            if(regRs == 31)
                return true;
        }
    }
    return false;
}
bool Hazard::isEX_MEMForward_rt(unsigned int regRt){
    Instruction inst = pipelineEX_MEMOut.inst;
    bool regWrite = false;
    if(inst.type == 'R'){
        if(inst.func == ADD || inst.func == ADDU || inst.func == SUB || inst.func == AND ||
           inst.func == OR || inst.func == XOR || inst.func == NOR || inst.func == NAND ||
           inst.func == SLT || inst.func == SLL || inst.func == SRL || inst.func == SRA ||
           inst.func == MFHI || inst.func == MFLO){
            regWrite = true;
        }
        if((regWrite == true)&&(inst.regRd != 0)&&(inst.regRd == regRt))
            return true;
    }
    else if(inst.type == 'I'){
        if(inst.opCode == ADDI || inst.opCode == ADDIU || inst.opCode == LUI || inst.opCode == ANDI ||
           inst.opCode == ORI || inst.opCode == NORI || inst.opCode == SLTI){
            regWrite = true;
        }
        if((regWrite == true)&&(inst.regRt != 0)&&(inst.regRt == regRt))
            return true;
    }
    else if(inst.type == 'J'){
        if(inst.opCode == JAL){
            if(regRt == 31)
                return true;
        }
    }
    return false;
}
/*
load use :
    ex   lw $3, 0($2)
         or $1, $3, $4
    need stall then forward
*/
bool Hazard::isMEM_WBForward_rs(unsigned int regRs){
    Instruction inst = pipelineMEM_WBOut.inst;
    bool regWrite = false;
    if(inst.type == 'R'){
        if(inst.func == ADD || inst.func == ADDU || inst.func == SUB || inst.func == AND ||
           inst.func == OR || inst.func == XOR || inst.func == NOR || inst.func == NAND ||
           inst.func == SLT || inst.func == SLL || inst.func == SRL || inst.func == SRA ||
           inst.func == MFHI || inst.func == MFLO){
            regWrite = true;
        }
        if((regWrite == true)&&(inst.regRd != 0)&&(inst.regRd == regRs)&&(!isEX_MEMForward_rs(regRs)))
            return true;
        else
            return false;
    }
    else if(inst.type == 'I'){
        if(inst.opCode == ADDI || inst.opCode == ADDIU || inst.opCode == LUI || inst.opCode == ANDI ||
           inst.opCode == ORI || inst.opCode == NORI || inst.opCode == SLTI ||
           inst.opCode == LW || inst.opCode == LH || inst.opCode == LHU ||
           inst.opCode == LB || inst.opCode == LBU){
            regWrite = true;
        }
        if((regWrite == true)&&(inst.regRt != 0)&&(inst.regRt == regRs))
            return true;
    }
    else if(inst.type == 'J'){
        if(inst.opCode == JAL){
            if(regRs == 31)
                return true;
        }
    }
    return false;
}
bool Hazard::isMEM_WBForward_rt(unsigned int regRt){
    Instruction inst = pipelineMEM_WBOut.inst;
    bool regWrite = false;
    if(inst.type == 'R'){
        if(inst.func == ADD || inst.func == ADDU || inst.func == SUB || inst.func == AND ||
           inst.func == OR || inst.func == XOR || inst.func == NOR || inst.func == NAND ||
           inst.func == SLT || inst.func == SLL || inst.func == SRL || inst.func == SRA ||
           inst.func == MFHI || inst.func == MFLO){
            regWrite = true;
        }
        if((regWrite == true)&&(inst.regRd != 0)&&(inst.regRd == regRt)&&(!isEX_MEMForward_rt(regRt)))
            return true;
    }
    else if(inst.type == 'I'){
        if(inst.opCode == ADDI || inst.opCode == ADDIU || inst.opCode == LUI || inst.opCode == ANDI ||
           inst.opCode == ORI || inst.opCode == NORI || inst.opCode == SLTI||
           inst.opCode == LW || inst.opCode == LH || inst.opCode == LHU ||
           inst.opCode == LB || inst.opCode == LBU){
            regWrite = true;
        }
        if((regWrite == true)&&(inst.regRt != 0)&&(inst.regRt == regRt)&&(!isEX_MEMForward_rt(regRt)))
            return true;
    }
     else if(inst.type == 'J'){
        if(inst.opCode == JAL){
            if(regRt == 31)
                return true;
        }
    }
    return false;
}
bool Hazard::isStallForR(){
    Instruction instForID_EX = pipelineID_EXOut.inst;
    Instruction instForEX_MEM = pipelineEX_MEMOut.inst;

	if(pipelineID_EXIn.inst.func == JR){
        if(instForID_EX.type == 'R'){
            if((instForID_EX.regRd != 0) && (instForID_EX.regRd == pipelineID_EXIn.inst.regRs) && (instForID_EX.func != JR &&
                instForID_EX.func != MULT && instForID_EX.func != MULTU)){
                return true;
            }
        }
        else if(instForID_EX.type == 'I'){
            if((instForID_EX.regRt != 0) && (instForID_EX.regRt == pipelineID_EXIn.inst.regRs) && (instForID_EX.opCode != SW &&
                instForID_EX.opCode != SH && instForID_EX.opCode != SB && instForID_EX.opCode != BEQ && instForID_EX.opCode != BNE && 				 instForID_EX.opCode != BGTZ)){
                return true;
            }
        }
		if(instForEX_MEM.type == 'I'){
            if((instForEX_MEM.regRt != 0) && (instForEX_MEM.regRt == pipelineID_EXIn.inst.regRs || 
				instForEX_MEM.regRt == pipelineID_EXIn.inst.regRt) && (instForEX_MEM.opCode == LW || instForEX_MEM.opCode == LH || 
				instForEX_MEM.opCode == LHU || instForEX_MEM.opCode == LB || instForEX_MEM.opCode == LBU)){
                return true;
            }
        }
        return false;
    }
	else if(pipelineID_EXIn.inst.func == MFLO || pipelineID_EXIn.inst.func == MFHI){
        return false;
    }
	else if(pipelineID_EXIn.inst.func == SLL || pipelineID_EXIn.inst.func == SRL || pipelineID_EXIn.inst.func == SRA){
        if((instForID_EX.regRt != 0) && (instForID_EX.regRt == pipelineID_EXIn.inst.regRt) &&
           (instForID_EX.opCode == LW || instForID_EX.opCode == LH || instForID_EX.opCode == LHU || instForID_EX.opCode == LB || instForID_EX.opCode == LBU)){
            return true;
           }
        return false;
    }
    else{
    	if((instForID_EX.regRt != 0) && (instForID_EX.regRt ==  pipelineID_EXIn.inst.regRs || instForID_EX.regRt == pipelineID_EXIn.inst.regRt) &&
           (instForID_EX.opCode == LW || instForID_EX.opCode == LH || instForID_EX.opCode == LHU || instForID_EX.opCode == LB || instForID_EX.opCode == LBU)){
            return true;
        }
        return false;
	}
}
bool Hazard::isStallForI(){
    Instruction instForID_EX = pipelineID_EXOut.inst;
    Instruction instForEX_MEM = pipelineEX_MEMOut.inst;

    if(pipelineID_EXIn.inst.opCode == BEQ || pipelineID_EXIn.inst.opCode == BNE || pipelineID_EXIn.inst.opCode == BGTZ){
        /*load use need stall one cycle*/
        if(instForID_EX.type == 'R'){
        	if(pipelineID_EXIn.inst.opCode == BEQ || pipelineID_EXIn.inst.opCode == BNE){
                if((instForID_EX.regRd != 0) && (instForID_EX.regRd == pipelineID_EXIn.inst.regRs || instForID_EX.regRd == pipelineID_EXIn.inst.regRt) &&
                (instForID_EX.func != JR && instForID_EX.func != MULT && instForID_EX.func != MULTU)){
                    return true;
                }
            }
            else if(pipelineID_EXIn.inst.opCode == BGTZ){
                if((instForID_EX.regRd != 0) && (instForID_EX.regRd == pipelineID_EXIn.inst.regRs) &&
                (instForID_EX.func != JR && instForID_EX.func != MULT && instForID_EX.func != MULTU)){
                    return true;
                }
            }
		}
        else if(instForID_EX.type == 'I'){
            if(pipelineID_EXIn.inst.opCode == BEQ || pipelineID_EXIn.inst.opCode == BNE){
				if((instForID_EX.regRt != 0) && (instForID_EX.regRt == pipelineID_EXIn.inst.regRs || 
					instForID_EX.regRt == pipelineID_EXIn.inst.regRt) && (instForID_EX.opCode != SW && instForID_EX.opCode != SH && 
					instForID_EX.opCode != SB && instForID_EX.opCode != BEQ && instForID_EX.opCode != BNE && 
					instForID_EX.opCode != BGTZ)){
                	return true;
            	}
			}
			else if(pipelineID_EXIn.inst.opCode == BGTZ){
                if((instForID_EX.regRt != 0) && (instForID_EX.regRt == pipelineID_EXIn.inst.regRs) &&
                    (instForID_EX.opCode != SW && instForID_EX.opCode != SH && instForID_EX.opCode != SB && 
					 instForID_EX.opCode != BEQ && instForID_EX.opCode != BNE && instForID_EX.opCode != BGTZ)){
                    return true;
                }
            }
        }
        /*branch right after load need stall two cycle*/
        if(instForEX_MEM.type == 'I'){
        	if(pipelineID_EXIn.inst.opCode == BEQ || pipelineID_EXIn.inst.opCode == BNE){
                if((instForEX_MEM.regRt != 0) && (instForEX_MEM.regRt == pipelineID_EXIn.inst.regRs || instForEX_MEM.regRt == pipelineID_EXIn.inst.regRt) &&
                (instForEX_MEM.opCode == LW || instForEX_MEM.opCode == LH || instForEX_MEM.opCode == LHU || instForEX_MEM.opCode == LB
                 || instForEX_MEM.opCode == LBU)){
                    return true;
                }
            }
            else if(pipelineID_EXIn.inst.opCode == BGTZ){
                if((instForEX_MEM.regRt != 0) && (instForEX_MEM.regRt == pipelineID_EXIn.inst.regRs) &&
                (instForEX_MEM.opCode == LW || instForEX_MEM.opCode == LH || instForEX_MEM.opCode == LHU || instForEX_MEM.opCode == LB
                 || instForEX_MEM.opCode == LBU)){
                    return true;
                }
            }
		}
        return false;
    }
	else if(pipelineID_EXIn.inst.opCode == LUI){
        return false;
    }
    else{
        if(instForID_EX.opCode == LW || instForID_EX.opCode == LH || instForID_EX.opCode == LHU || instForID_EX.opCode == LB || instForID_EX.opCode == LBU){
            if((instForID_EX.regRt != 0) && (pipelineID_EXIn.inst.regRs == instForID_EX.regRt)){
                return true;
            }
            else if((instForID_EX.regRt != 0) && (pipelineID_EXIn.inst.opCode == SW || pipelineID_EXIn.inst.opCode == SH ||
                    pipelineID_EXIn.inst.opCode == SB) && (pipelineID_EXIn.inst.regRt == instForID_EX.regRt)){
                return true;
            }
			return false;
        }
        return false;
    }

}

