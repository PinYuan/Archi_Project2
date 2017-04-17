#include <bits/stdc++.h>
#include "errorDetect.h"
#define MULT 0x18
#define MULTU 0x19
#define MFHI 0x10
#define MFLO 0x12
#define LW 0x23
#define LH 0x21
#define LHU 0x25
#define SW 0x2b
#define SH 0x29
using namespace std;

extern unsigned int halt;

ErrorDetect::ErrorDetect(){
    cycle = 0;
    mult = mf = 0;
    FILE* fptr;
    fptr = fopen("error_dump.rpt", "w");
    fclose(fptr);
}
void ErrorDetect::setCycle(int cycle){
    this->cycle = cycle;
}
void ErrorDetect::writeToRegister0(unsigned int writeToReg){
    if(writeToReg == 0){
        FILE* fptr;
        fptr = fopen("error_dump.rpt", "a");
        char buffer[35];
        fprintf(fptr, "In cycle %d: Write $0 Error\n", cycle+1);
        fclose(fptr);
    }
}
void ErrorDetect::numberOverflow(unsigned int data1, unsigned int data2, unsigned int output){
    unsigned int signBit1, signBit2, signBitOut;
    signBit1 = data1 >> 31;
    signBit2 = data2 >> 31;
    signBitOut = output >> 31;
    if((signBit1 ^ signBit2 == 0) && (signBit1 != signBitOut)){
        FILE* fptr;
        fptr = fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Number Overflow\n", cycle+1);
        fclose(fptr);
    }
}
void ErrorDetect::overwriteHILORegister(unsigned int func){//every time call mult(u) and mfhi(lo)
    if(func == MULT || func == MULTU){//mult(u)
        mult++;
        if(mult != 1){
            if(mf == 0){
                FILE* fptr;
                fptr = fopen("error_dump.rpt", "a");
                fprintf(fptr , "In cycle %d: Overwrite HI-LO registers\n", cycle+1);
                fclose(fptr);
            }
            else mf = 0;
        }
    }
    else if(func == MFHI || func == MFLO){
        if(mult != 0)
            mf++;
    }
}
void ErrorDetect::memoryAddressOverflow(unsigned int opCode, unsigned int address){
    unsigned int maxAddress(0);
    if(opCode == LW || opCode == SW)//word
        maxAddress = address+3;
    else if(opCode == LH || opCode == LHU || opCode == SH)//half
        maxAddress = address+1;
    else//byte
        maxAddress = address;
    if(address > 1023 || maxAddress > 1023){
        halt = 1;
        FILE* fptr;
        fptr = fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Address Overflow\n", cycle+1);
        fclose(fptr);
    }
}
void ErrorDetect::dataMisaligned(unsigned int opCode, unsigned int address){
    bool error = false;
	if(opCode == LW || opCode == SW){//word lw sw
        if(address%4 != 0)
           	error = true;
    }
    else if(opCode == LH || opCode == LHU || opCode == SH){//half lh lhu sh
        if(address%2 != 0)
            error = true;
    }
    if(error){
		halt = 1;
        FILE* fptr;
        fptr = fopen("error_dump.rpt", "a");
        fprintf(fptr , "In cycle %d: Misalignment Error\n", cycle+1);
        fclose(fptr);
    }
}

