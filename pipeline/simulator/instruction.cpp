#include <bits/stdc++.h>
#include "instruction.h"
using namespace std;

extern unsigned int pc;
extern unsigned int halt;

map<int, string> instFunc{
    {0x20, "ADD"}, {0x21, "ADDU"}, {0x22 ,"SUB"}, {0x24, "AND"}, {0x25, "OR"}, {0x26, "XOR"},
    {0x27, "NOR"}, {0x28, "NAND"}, {0x2A, "SLT"}, {0x00, "SLL"}, {0x02, "SRL"}, {0x03, "SRA"},
    {0x08, "JR"}, {0x18, "MULT"}, {0x19, "MULTU"}, {0x10, "MFHI"}, {0x12, "MFLO"},
};
map<int, string> instOpcode{
    {0x08, "ADDI"}, {0x09, "ADDIU"}, {0x23, "LW"}, {0x21, "LH"}, {0x25, "LHU"}, {0x20, "LB"},
    {0x24, "LBU"}, {0x2b, "SW"}, {0x29, "SH"}, {0x28, "SB"}, {0x0f, "LUI"}, {0x0c, "ANDI"},
    {0x0d, "ORI"},{0x0e, "NORI"}, {0x0a, "SLTI"}, {0x04, "BEQ"}, {0x05, "BNE"}, {0x07, "BGTZ"},

    {0x02, "J"}, {0x03, "JAL"}
};
Instruction::Instruction(void){
    completeInst = opCode = regRs = regRt = regRd =
    shamt = func = immediate = targetAddr = 0;
    type = '\0';
    name = "NOP";
}
Instruction::Instruction(unsigned int instruction){
    completeInst = instruction;
    opCode = instruction >> 26;
    regRs = (instruction >> 21)%32;
    regRt = (instruction >> 16)%32;
    regRd = (instruction >> 11)%32;
    shamt = (instruction >> 6)%32;
    func = instruction%64;
    immediate = instruction%65536;
    targetAddr = instruction%67108864;

    if(completeInst == 0){
        type = 'S';
        name = "NOP";
    }
    else if(opCode == 63){
        type = 'S';
        name = "HALT";
    }
    else if(opCode == 2 || opCode == 3){
        type = 'J';
        name = instOpcode[opCode];
    }
    else if(opCode == 0){
        type = 'R';
		if(func == 0 && regRt == 0 && regRd == 0 && shamt == 0){
            type = 'S';
            name = "NOP";
        }
        else if(instFunc.find(func) != instFunc.end())
            name = instFunc[func];
        else{
            printf("illegal instruction found at 0x%X\n", pc);
            halt = 0;
        }
	}
    else{
        type = 'I';
        if(instOpcode.find(opCode) != instOpcode.end())
            name = instOpcode[opCode];
        else{
            printf("illegal instruction found at 0x%X\n", pc);
            halt = 0;
        }
    }
}

