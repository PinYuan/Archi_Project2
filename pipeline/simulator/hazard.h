#ifndef HAZARD_H_INCLUDED
#define HAZARD_H_INCLUDED

class Hazard{
public:
	bool isEX_MEMForward_rs(unsigned int regRs);
    bool isEX_MEMForward_rt(unsigned int regRt);
    bool isMEM_WBForward_rs(unsigned int regRs);
    bool isMEM_WBForward_rt(unsigned int regRt);
	bool isStallForR();
    bool isStallForI();
};

#endif // HAZARD_H_INCLUDED


