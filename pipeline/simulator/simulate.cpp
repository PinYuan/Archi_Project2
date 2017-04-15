#include <bits/stdc++.h>
#include "simulate.h"
#include "pipelineReg.h"
#include "instruction.h"
#include "arithmetic.h"
#include "regFile.h"
#include "memory.h"
#include "ALU.h"
#include "errorDetect.h"

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

extern unsigned int pc, branchJrpc;
extern unsigned int initialSp;
extern unsigned int HI;
extern unsigned int LO;
extern unsigned int halt;

extern InstructionMemory IM;
extern DataMemory DM;
extern RegisterFile RF;
extern ALU ALU1;
extern ErrorDetect ED;

extern PipelineIF_ID pipelineIF_IDIn, pipelineIF_IDOut;
extern PipelineID_EX pipelineID_EXIn, pipelineID_EXOut;
extern PipelineEX_MEM pipelineEX_MEMIn, pipelineEX_MEMOut;
extern PipelineMEM_WB pipelineMEM_WBIn, pipelineMEM_WBOut;
extern PipelineWB pipelineWB;

void Simulator::instructionFetch(){
    pipelineIF_IDIn.inst = IM.readInstMemory(pc);
}
void Simulator::instructionDecode(){
    Instruction inst(pipelineIF_IDOut.inst);

    unsigned int signExtendOut = signExtend(inst.immediate, 16);
    pipelineID_EXIn.signExtendImme = signExtendOut;
    pipelineID_EXIn.unsignExtendImme = inst.immediate;
    pipelineID_EXIn.inst = inst;

    branchJr = false;
    stall = false;
    flush = false;
    forward_EX_MEM_rs_ID = false;
    forward_EX_MEM_rt_ID = false;
    forward_MEM_WB_rs_ID = false;
    forward_MEM_WB_rt_ID = false;

    if(inst.type == 'J'){
        if(inst.opCode == JAL){
        	pipelineID_EXIn.rsData = pc;
		}
        branchJrpc = ((pc & 0xf0000000) | (inst.targetAddr << 2));
        branchJr = true;
        flush = true;
    }
    else if(inst.type == 'R'){
        /*
        if load use need stall
        only jr need forward in ID
        */
        if(inst.func == JR){
            if(!hazard.isStallForR()){
                RF.readWrite(inst.regRs, inst.regRt, 0, 0, false);
                unsigned int data1;

                if(hazard.isEX_MEMForward_rs(inst.regRs)) data1 = pipelineEX_MEMOut.ALUOut, forward_EX_MEM_rs_ID = true;
                else data1 = RF.readData1;

           		branchJrpc = data1;
                branchJr = true;
                flush = true;	
			 }
            else stall = true;
        }
        else{
            if(!hazard.isStallForR()){
                RF.readWrite(inst.regRs, inst.regRt, 0, 0, false);
                pipelineID_EXIn.rsData = RF.readData1;
                pipelineID_EXIn.rtData = RF.readData2;
            }
            else stall = true;
        }
    }
    else if(inst.type == 'I'){
        /*
        load use need stall
        only branch need forward in ID
        */
        if(inst.opCode == BEQ || inst.opCode == BNE || inst.opCode == BGTZ){
            if(!hazard.isStallForI()){
                RF.readWrite(inst.regRs, inst.regRt, 0, 0, false);
                unsigned int data1, data2;

                if(hazard.isEX_MEMForward_rs(inst.regRs)) data1 = pipelineEX_MEMOut.ALUOut, forward_EX_MEM_rs_ID = true;
                else data1 = RF.readData1;
                if(hazard.isEX_MEMForward_rt(inst.regRt)) data2 = pipelineEX_MEMOut.ALUOut, forward_EX_MEM_rt_ID = true;
                else data2 = RF.readData2;
                /*judge*/
                if(inst.opCode == BEQ){
                    if(data1 == data2){
                        branchJrpc = pc + (signExtendOut << 2);
                        branchJr = true;
                        flush = true;
                    }
                }
                else if(inst.opCode == BNE){
                    if(data1 != data2){
                        branchJrpc = pc + (signExtendOut << 2);
                        branchJr = true;
                        flush = true;
                    }
                }
                else if(inst.opCode == BGTZ){
                    if(data1>>31 == 0){
                        branchJrpc = pc + (signExtendOut << 2);
                        branchJr = true;
                        flush = true;
                    }
                }
            }
            else stall = true;
        }
        else{
            if(!hazard.isStallForI()){
                RF.readWrite(inst.regRs, inst.regRt, 0, 0, false);
                pipelineID_EXIn.rsData = RF.readData1;
                pipelineID_EXIn.rtData = RF.readData2;
            }
            else stall = true;
        }
    }
}
void Simulator::executeOp(){
    Instruction inst = pipelineID_EXOut.inst;
    pipelineEX_MEMIn.inst = pipelineID_EXOut.inst;
    pipelineEX_MEMIn.signExtendImme = pipelineID_EXOut.signExtendImme;
    pipelineEX_MEMIn.unsignExtendImme = pipelineID_EXOut.unsignExtendImme;

    forward_EX_MEM_rs_EX = false;
    forward_EX_MEM_rt_EX = false;
    forward_MEM_WB_rs_EX = false;
    forward_MEM_WB_rt_EX = false;
    unsigned int data1, data2;
	if(inst.type == 'J' && inst.opCode == JAL){
        pipelineEX_MEMIn.rsData = pipelineID_EXOut.rsData;
    }
    else if(inst.type == 'R' && inst.func != JR){
        //if forward
        if(hazard.isEX_MEMForward_rs(inst.regRs)) data1 = pipelineEX_MEMOut.ALUOut, forward_EX_MEM_rs_EX = true;
        else if(hazard.isMEM_WBForward_rs(inst.regRs)) data1 = pipelineMEM_WBOut.writeBackData, forward_MEM_WB_rs_EX = true;
        else data1 = pipelineID_EXOut.rsData;
        if(hazard.isEX_MEMForward_rt(inst.regRt)) data2 = pipelineEX_MEMOut.ALUOut, forward_EX_MEM_rt_EX = true;
        else if(hazard.isMEM_WBForward_rt(inst.regRt)) data2 =  pipelineMEM_WBOut.writeBackData, forward_MEM_WB_rt_EX = true;
        else data2 = pipelineID_EXOut.rtData;

		pipelineEX_MEMIn.rsData = data1;
        pipelineEX_MEMIn.rtData = data2;	

        if(inst.func == ADD || inst.func == ADDU){
            ALU1.ALUoperater(data1, data2, 0);
            if(inst.func == ADD)
                ED.numberOverflow(data1, data2, ALU1.ALUResult);
        }
        else if(inst.func == SUB){
            ALU1.ALUoperater(data1, data2, 1);
            bitset<32> data2ChangeSign(data2);
            ///need check
            data2ChangeSign = bitset<32>(data2ChangeSign.flip().to_ulong()+1);
            ED.numberOverflow(data1, data2ChangeSign.to_ulong(), ALU1.ALUResult);
        }
        else if(inst.func == AND){
            ALU1.ALUoperater(data1, data2, 2);
        }
        else if(inst.func == OR){
            ALU1.ALUoperater(data1, data2, 3);
        }
        else if(inst.func == XOR){
            ALU1.ALUoperater(data1, data2, 4);
        }
        else if(inst.func == NOR){
            ALU1.ALUoperater(data1, data2, 5);
        }
        else if(inst.func == NAND){
            ALU1.ALUoperater(data1, data2, 6);
        }
        else if(inst.func == SLT){
            ALU1.ALUoperater(data1, data2, 7);
        }
        else if(inst.func == SLL){
            ALU1.ALUoperater(data2, inst.shamt, 8);
        }
        else if(inst.func == SRL){
            ALU1.ALUoperater(data2, inst.shamt, 9);
        }
        else if(inst.func == SRA){
            ALU1.ALUoperater(data2, inst.shamt, 10);
        }
        else if(inst.func == MULT){
            ALU1.ALUoperater(data1, data2, 11);
            ED.overwriteHILORegister(MULT);
            HI = (ALU1.ALUResult64 >> 32)%4294967296;
            LO = ALU1.ALUResult64%4294967296;
        }
        else if(inst.func == MULTU){
            ALU1.ALUoperater(data1, data2, 12);
            ED.overwriteHILORegister(MULTU);
            HI = (ALU1.ALUResult64 >> 32)%4294967296;
            LO = ALU1.ALUResult64%4294967296;
        }
        else if(inst.func == MFHI){
            pipelineEX_MEMIn.HI = HI;
            ED.overwriteHILORegister(MFHI);
        }
        else if(inst.func == MFLO){
            pipelineEX_MEMIn.LO = LO;
            ED.overwriteHILORegister(MFLO);
        }
    }
    else if(inst.type == 'I' && (inst.opCode != BEQ && inst.opCode != BNE && inst.opCode != BGTZ)){
        if(hazard.isEX_MEMForward_rs(inst.regRs)) data1 = pipelineEX_MEMOut.ALUOut, forward_EX_MEM_rs_EX = true;
        else if(hazard.isMEM_WBForward_rs(inst.regRs)) data1 = pipelineMEM_WBOut.writeBackData, forward_MEM_WB_rs_EX = true;
        else data1 = pipelineID_EXOut.rsData;
		//for store
		if(inst.opCode == SW || inst.opCode == SH || inst.opCode == SB){
            if(hazard.isEX_MEMForward_rt(inst.regRt)) data2 = pipelineEX_MEMOut.ALUOut, forward_EX_MEM_rt_EX = true;
            else if(hazard.isMEM_WBForward_rt(inst.regRt)) data2 =  pipelineMEM_WBOut.writeBackData, forward_MEM_WB_rt_EX = true;
            else data2 = pipelineID_EXOut.rtData;
        }
		
		pipelineEX_MEMIn.rsData = data1;
        pipelineEX_MEMIn.rtData = data2;

        if(inst.opCode == ADDI || inst.opCode == ADDIU ||
            inst.opCode == LW || inst.opCode == LH || inst.opCode == LHU || inst.opCode == LB ||
            inst.opCode == LBU ||
            inst.opCode == SW || inst.opCode == SH || inst.opCode == SB){
            ALU1.ALUoperater(data1, pipelineEX_MEMIn.signExtendImme, 0);
            if(inst.opCode == ADDI ||
                inst.opCode == LW || inst.opCode == LH || inst.opCode == LHU || inst.opCode == LB ||
                inst.opCode == SW || inst.opCode == SH || inst.opCode == SB)
                ED.numberOverflow(data1, pipelineEX_MEMIn.signExtendImme, ALU1.ALUResult);
        }
        else if(inst.opCode == LUI){
            ALU1.ALUoperater(pipelineEX_MEMIn.unsignExtendImme, 0, 13);
        }
        else if(inst.opCode == ANDI){
            ALU1.ALUoperater(data1, pipelineEX_MEMIn.unsignExtendImme, 2);
        }
        else if(inst.opCode == ORI){
            ALU1.ALUoperater(data1, pipelineEX_MEMIn.unsignExtendImme, 3);
        }
        else if(inst.opCode == NORI){
            ALU1.ALUoperater(data1, pipelineEX_MEMIn.unsignExtendImme, 5);
        }
        else if(inst.opCode == SLTI){
            ALU1.ALUoperater(data1, pipelineEX_MEMIn.signExtendImme, 7);
        }
    }
    pipelineEX_MEMIn.ALUOut = ALU1.ALUResult;
    pipelineEX_MEMIn.ALUOut64 = ALU1.ALUResult64;
}
void Simulator::memoryAccess(){
    Instruction inst = pipelineEX_MEMOut.inst;
    pipelineMEM_WBIn.inst = pipelineEX_MEMOut.inst;
    pipelineMEM_WBIn.rtData = pipelineEX_MEMOut.rtData;
    pipelineMEM_WBIn.ALUOut = pipelineEX_MEMOut.ALUOut;
    pipelineMEM_WBIn.HI = pipelineEX_MEMOut.HI;
    pipelineMEM_WBIn.LO = pipelineEX_MEMOut.LO;

	if(inst.type == 'J' && inst.opCode == JAL){
        pipelineMEM_WBIn.writeBackData = pipelineEX_MEMOut.rsData;
    }
    else if(inst.type == 'R'){
        pipelineMEM_WBIn.writeBackData = pipelineEX_MEMOut.ALUOut;
    }
    else if(inst.type == 'I'){
        if(inst.opCode == LW || inst.opCode == LH || inst.opCode == LHU || inst.opCode == LB ||
            inst.opCode == LBU){
            ED.memoryAddressOverflow(inst.opCode, pipelineMEM_WBIn.ALUOut);
            ED.dataMisaligned(inst.opCode, pipelineMEM_WBIn.ALUOut);
            if(halt) return;

            if(inst.opCode == LW)
                pipelineMEM_WBIn.writeBackData = DM.readWriteMemory(pipelineMEM_WBIn.ALUOut, 0, true, false, WORD);
            else if(inst.opCode == LH || inst.opCode == LHU){
               	if(inst.opCode == LH){
                    unsigned int data = DM.readWriteMemory(pipelineMEM_WBIn.ALUOut, 0, true, false, HALF);
                    if((data>>15) == 1)
                        pipelineMEM_WBIn.writeBackData = 0xffff0000 | data;
                    else
                        pipelineMEM_WBIn.writeBackData = data;
                } 
				else
                    pipelineMEM_WBIn.writeBackData = DM.readWriteMemory(pipelineMEM_WBIn.ALUOut, 0, true, false, HALF);
            }
            else if(inst.opCode == LB || inst.opCode == LBU){
                if(inst.opCode == LB){
                    unsigned int data = DM.readWriteMemory(pipelineMEM_WBIn.ALUOut, 0, true, false, BYTE);
                    if((data>>7) == 1)
                        pipelineMEM_WBIn.writeBackData = 0xffffff00 | data;
                    else
                        pipelineMEM_WBIn.writeBackData = data;
                }
				else
                    pipelineMEM_WBIn.writeBackData = DM.readWriteMemory(pipelineMEM_WBIn.ALUOut, 0, true, false, BYTE);
            }
        }
        else if(inst.opCode == SW || inst.opCode == SH || inst.opCode == SB){
            ED.memoryAddressOverflow(inst.opCode, pipelineMEM_WBIn.ALUOut);
            ED.dataMisaligned(inst.opCode, pipelineMEM_WBIn.ALUOut);
            if(halt) return;

            if(inst.opCode == SW)
                DM.readWriteMemory(pipelineMEM_WBIn.ALUOut, pipelineMEM_WBIn.rtData, false, true, WORD);
            else if(inst.opCode == SH)
                DM.readWriteMemory(pipelineMEM_WBIn.ALUOut, pipelineMEM_WBIn.rtData, false, true, HALF);
            else if(inst.opCode == SB)
                DM.readWriteMemory(pipelineMEM_WBIn.ALUOut, pipelineMEM_WBIn.rtData, false, true, BYTE);
        }
        else
            pipelineMEM_WBIn.writeBackData = pipelineEX_MEMOut.ALUOut;
    }
}
void Simulator::writeBack(){
    Instruction inst = pipelineMEM_WBOut.inst;
    pipelineWB.inst = pipelineMEM_WBOut.inst;
    pipelineWB.HI = pipelineMEM_WBOut.HI;
    pipelineWB.LO = pipelineMEM_WBOut.LO;
    pipelineWB.writeBackData = pipelineMEM_WBOut.writeBackData;

	if(inst.type == 'J'){
        if(inst.opCode == JAL){
            RF.readWrite(0, 0, 31, pipelineWB.writeBackData, true);
        }
    }
    else if(inst.type == 'R'){
        if(inst.func == ADD || inst.func == ADDU || inst.func == SUB || inst.func == AND ||
           inst.func == OR || inst.func == XOR || inst.func == NOR || inst.func == NAND ||
           inst.func == SLT || inst.func == SLL || inst.func == SRL || inst.func == SRA){
            RF.readWrite(0, 0, inst.regRd, pipelineWB.writeBackData, true);
            ED.writeToRegister0(inst.regRd);
        }
        else if(inst.func == MFHI){
            RF.readWrite(0, 0, inst.regRd, pipelineWB.HI, true);
            ED.writeToRegister0(inst.regRd);
        }
        else if(inst.func == MFLO){
            RF.readWrite(0, 0, inst.regRd, pipelineWB.LO, true);
            ED.writeToRegister0(inst.regRd);
        }
    }
    else if(inst.type == 'I'){
        if(inst.opCode == LW || inst.opCode == LH || inst.opCode == LHU || inst.opCode == LB ||
            inst.opCode == LBU){
            RF.readWrite(0, 0, inst.regRt, pipelineWB.writeBackData, true);
            ED.writeToRegister0(inst.regRt);
        }
        else if(inst.opCode == ADDI || inst.opCode == ADDIU || inst.opCode == LUI ||  inst.opCode == ANDI || inst.opCode == ORI ||
                inst.opCode == NORI || inst.opCode == SLTI){
            RF.readWrite(0, 0, inst.regRt, pipelineWB.writeBackData, true);
            ED.writeToRegister0(inst.regRt);
        }
    }
}
void Simulator::updatePipelineReg(){
    //stall
    if(stall){
        pipelineID_EXOut.inst = Instruction(0);
    }
    //flush
    else if(flush){
        pipelineIF_IDOut.inst = 0;
        pipelineID_EXOut = pipelineID_EXIn;

    }
    else{
        pipelineIF_IDOut = pipelineIF_IDIn;
        pipelineID_EXOut = pipelineID_EXIn;
    }
    pipelineEX_MEMOut = pipelineEX_MEMIn;
    pipelineMEM_WBOut = pipelineMEM_WBIn;
}

