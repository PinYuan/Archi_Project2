#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED
#include <bits/stdc++.h>
using namespace std;

class InstructionMemory{
private:
    unsigned char instMemory[1024];
    unsigned int numOfInst;
public:
    InstructionMemory();

    unsigned int readInstMemory(unsigned int readAddress);

    void outputInstMemory();
};

enum Type{WORD, HALF, BYTE};

class DataMemory{
private:
    unsigned char dataMemory[1024];
    unsigned int numOfData;
public:
    DataMemory();

    unsigned int readWriteMemory(unsigned int address, unsigned int writeData, bool memRead, bool memWrite, Type type);

    void outputDataMemory();
};

#endif // MEMORY_H_INCLUDED
