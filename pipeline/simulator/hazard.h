#ifndef HAZARD_H_INCLUDED
#define HAZARD_H_INCLUDED

class Hazard{
public:
    bool isEX_MEMForward_rs();
    bool isEX_MEMForward_rt();
    bool isMEM_WBForward_rs();
    bool isMEM_WBForward_rt();
};

#endif // HAZARD_H_INCLUDED


