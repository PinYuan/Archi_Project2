fndef ERRORDETECT_H_INCLUDED
#define ERRORDETECT_H_INCLUDED
using namespace std;

class ErrorDetect{
private:
    int mult;//mult(u)
    int mf;//mfhi mflo
    int cycle;
public:
    ErrorDetect();
    void setCycle(int cycle);
    void writeToRegister0(unsigned int writeToReg);
    void numberOverflow(unsigned int data1, unsigned int data2, unsigned int output);
    void overwriteHILORegister(unsigned int func);
    void memoryAddressOverflow(unsigned int opCode, unsigned int address);
    void dataMisaligned(unsigned int opCode, unsigned int address);
};


#endif // ERRORDETECT_H_INCLUDED

