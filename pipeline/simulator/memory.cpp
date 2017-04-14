#include <bits/stdc++.h>
#include "memory.h"
//#include "ErrorDetect.h"
using namespace std;

extern unsigned int pc;
extern unsigned int initialSp;

InstructionMemory::InstructionMemory(){
    FILE *fptr;
    fptr = fopen("testcase/iimage.bin" , "rb");
    if(!fptr){printf("open file error\n");return;}

    for(int i=0;i<1024;i++)
        instMemory[i] = '\0';

    unsigned char c;

    pc = 0;
    for(int i=0;i<4;i++){
        c = fgetc(fptr);
        pc += c << (8*(3-i));
    }

    numOfInst = 0;
    for(int i=0;i<4;i++){
        c = fgetc(fptr);
        numOfInst += c << (8*(3-i));
    }

    for(int i=0;i<numOfInst;i++){
        for(int j=0;j<4;j++){
            c = fgetc(fptr);
            instMemory[pc+4*i+j] = c;
        }
    }

    fclose(fptr);
}

unsigned int InstructionMemory::readInstMemory(unsigned int readAddress){
    unsigned int b0 = instMemory[readAddress];
    unsigned int b1 = instMemory[readAddress+1];
    unsigned int b2 = instMemory[readAddress+2];
    unsigned int b3 = instMemory[readAddress+3];

    unsigned int inst = (b0 << 24)|(b1 << 16)|(b2 << 8)|b3;
    return inst;
}

void InstructionMemory::outputInstMemory(){
    FILE* fptr;
    fptr = fopen("instMemOut.out", "w");
    if(!fptr){printf("open file error\n");return ;}

    unsigned char c;
    char buffer[20] = {'\0'};
    unsigned int inst;

    for(int i=0;i<numOfInst;i++){
        inst = 0;
        for(int j=0;j<4;j++){
            c = instMemory[pc+i*4+j];
            inst += c << (8*(3-j));
        }
        sprintf(buffer, "0x%08X\n", inst);
        fwrite(buffer, strlen(buffer), 1, fptr);
    }
    fclose(fptr);
}

DataMemory::DataMemory(){
    FILE *fptr;
    fptr = fopen("testcase/dimage.bin" , "rb");
    if(!fptr){printf("open file error\n");return ;}

    unsigned char c;

    initialSp = 0;
    for(int i=0;i<4;i++){
        c = fgetc(fptr);
        initialSp += c << (8*(3-i));
    }

    numOfData = 0;
    for(int i=0;i<4;i++){
        c = fgetc(fptr);
        numOfData += c << (8*(3-i));
    }

    for(int i=0;i<numOfData;i++){
        for(int j=0;j<4;j++){
            c = fgetc(fptr);
            dataMemory[4*i+j] = c;
        }
    }

    fclose(fptr);
}

unsigned int DataMemory::readWriteMemory(unsigned int address, unsigned int writeData, bool memRead, bool memWrite, Type type){
    if(memRead == true){
        if(type == WORD){
            return (unsigned int)((dataMemory[address] << 24) | (dataMemory[address+1] << 16) | (dataMemory[address+2] << 8)
                                  | dataMemory[address+3]);
        }
        else if(type == HALF){
            return (unsigned int)((dataMemory[address] << 8) | dataMemory[address+1]);
        }
        else if(type == BYTE){
            return (unsigned int)dataMemory[address];
        }
    }
    if(memWrite == true){
        if(type == WORD){
            dataMemory[address] = (writeData&0XFF000000) >> 24;
            dataMemory[address+1] = (writeData&0X00FF0000) >> 16;
            dataMemory[address+2] = (writeData&0X0000FF00) >> 8;
            dataMemory[address+3] = (writeData&0X000000FF);
        }
        else if(type == HALF){
            dataMemory[address] = (writeData&0X0000FF00) >> 8;
            dataMemory[address+1] = (writeData&0X000000FF);
        }
        else if(type == BYTE){
            dataMemory[address] = (writeData&0X000000FF);
        }
    }
}

void DataMemory::outputDataMemory(){
    FILE* fptr;
    fptr = fopen("dataMemOut.out", "w");
    if(!fptr){printf("open file error\n");return;}

    unsigned char c;
    char buffer[20] = {'\0'};
    unsigned int value;

    for(int i=0;i<numOfData;i++){
        value = 0;
        for(int j=0;j<4;j++){
            c = dataMemory[i*4+j];
            value += c << (8*(3-j));
        }
        sprintf(buffer, "0x%08X\n", value);
        fwrite(buffer, strlen(buffer), 1, fptr);
    }
    fclose(fptr);
}

