#ifndef PIPERPT_H_INCLUDED
#define PIPERPT_H_INCLUDED

void hazardReport(bool flush, bool stall, bool forward_EX_MEM_rs_ID ,bool forward_MEM_WB_rs_ID, bool forward_EX_MEM_rt_ID,
                  bool forward_MEM_WB_rt_ID, bool forward_EX_MEM_rs_EX, bool forward_MEM_WB_rs_EX, bool forward_EX_MEM_rt_EX,
                  bool forward_MEM_WB_rt_EX);

#endif // PIPERPT_H_INCLUDED

