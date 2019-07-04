//
// Created by 57402 on 2019/7/3.
//

#ifndef RISCV_RISCV_H
#define RISCV_RISCV_H
#include "operator.hpp"
const int maxn =0x3fffff;
unsigned char memory[maxn]={0};//内存
int reg[32];//寄存器
int PC=0;//PC寄存器
int length=0;


#endif //RISCV_RISCV_H
