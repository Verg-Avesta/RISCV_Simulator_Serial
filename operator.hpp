//
// Created by 57402 on 2019/7/2.
//

#ifndef RISCV_OPERATOR_H
#define RISCV_OPERATOR_H

#include "RISCV.hpp"
int signedExtend(int data,int bits){
    if(data&(1<<bits))
        data|=0xffffffff>>bits<<bits;
    return data;
}
class inst{
    public:
        /*int rs1,rs2,rd,imm,f3,f7,opcode,code;
        virtual void ID() const =0;
        virtual void EX() const=0;
        virtual void MEM() const=0;
        virtual void WB() const =0;*/
};
class U:public inst{
public:
    int rd,imm,opcode,code;
    int result;
    void ID(){
        opcode=code&((1<<7)-1);
        rd=(code>>7)&((1<<5)-1);
        imm=(code>>12);
        if(opcode!=111)imm=(imm<<12);
        else{
            int a,b,c,d;
            d=imm&((1<<8)-1);
            c=(imm>>8)&1;
            b=(imm>>9)&((1<<10)-1);
            a=(imm>>19);
            imm=(a<<20)+(b<<1)+(c<<11)+(d<<12);
            imm=signedExtend(imm,20);
        }
    }
    void EX(){
        switch(opcode){
            case 55:result=imm;break;//LUI
            case 23:result=imm;break;//AUIPC
            case 111:result=imm;break;//JAL
        }
    }
    void MEM(){
        switch(opcode){
            case 55:break;//LUI
            case 23:break;//AUIPC
            case 111:break;//JAL
        }
    }
    void WB() {
        switch (opcode) {
            case 55:if(rd!=0)reg[rd]=result;break;//LUI
            case 23:PC+=imm-4;if(rd!=0)reg[rd]=PC;break;//AUIPC
            case 111:if(rd!=0)reg[rd]=PC;PC+=imm-4;break;//JAL
        }
    }
};
class R:public inst{
public:
    int rs1,rs2,rd,f3,f7,opcode,code;
    int result;
    void ID(){
        opcode=code&((1<<7)-1);
        rd=(code>>7)&((1<<5)-1);
        f3=(code>>12)&((1<<3)-1);
        rs1=(code>>15)&((1<<5)-1);
        rs2=(code>>20)&((1<<5)-1);
        f7=(code>>25);
    }
    void EX(){
        switch(f3){
            case 0:
                if(f7==0) result=reg[rs1]+reg[rs2];//ADD
                else result=reg[rs1]-reg[rs2];//SUB
                break;
            case 1:result=reg[rs2]&((1<<5)-1);break;//SLL
            case 2:result=(reg[rs1]<reg[rs2]);break;//SLT
            case 3:result=((unsigned)reg[rs1]<(unsigned)reg[rs2]);break;//SLTU
            case 4:result=(reg[rs1]^reg[rs2]);break;//XOR
            case 5:result=reg[rs2]&((1<<5)-1); //SRL&SRA
                break;
            case 6: result=reg[rs1]|reg[rs2];break;//OR
            case 7: result=reg[rs1]&reg[rs2];break;//AND
        }
    }
    void MEM() {
        switch(f3){
            case 0:break;//ADD&SUB
            case 1:break;//SLL
            case 2:break;//SLT
            case 3:break;//SLTU
            case 4:break;//XOR
            case 5:break;//SRA&SRL
            case 6:break;//OR
            case 7:break;//AND
        }
    }
    void WB() {
        if(rd!=0)
        { switch (f3) {
            case 0:reg[rd]=result;break;//ADD&SUB
            case 1:reg[rd]=reg[rs1]<<result;break;//SLL
            case 2:reg[rd]=result;break;//SLT
            case 3:reg[rd]=result;break;//SLTU
            case 4:reg[rd]=result;break;//XOR
            case 5:
                if (f7 == 0){reg[rd]=(unsigned)reg[rs1]>>result;reg[rd]=(int)reg[rd];}//SRL
                else reg[rd]=reg[rs1]>>result;//SRA
                break;
            case 6:reg[rd]=result;break;//OR
            case 7:reg[rd]=result;break;//AND
             }
        }
    }
};
class S:public inst{
public:
    int rs1,rs2,rd,imm,f3,opcode,code;
    int result;
    void ID(){
        opcode=code&((1<<7)-1);
        rd=(code>>7)&((1<<5)-1);//靠右的imm
        f3=(code>>12)&((1<<3)-1);
        rs1=(code>>15)&((1<<5)-1);
        rs2=(code>>20)&((1<<5)-1);
        imm=(code>>25);//最后的imm
        if(opcode==35){
            imm=(imm<<5)+rd;
            imm=signedExtend(imm,11);
        }
        else{
            int a,b;
            a=rd&1;rd=(rd>>1);
            b=(imm>>6);imm=imm&((1<<6)-1);
            imm=(b<<12)+(a<<11)+(imm<<5)+(rd<<1);
            imm=signedExtend(imm,12);
        }
    }
    void EX(){
        if(opcode==99){
            switch(f3){
                case 0:result=(reg[rs1]==reg[rs2]);break;//BEQ
                case 1:result=(reg[rs1]!=reg[rs2]);break;//BNE
                case 4:result=(reg[rs1]<reg[rs2]);break;//BLT
                case 5:result=(reg[rs1]>=reg[rs2]);break;//BGE
                case 6:result=((unsigned)reg[rs1]<(unsigned)reg[rs2]);break;//BLTU
                case 7:result=((unsigned)reg[rs1]>=(unsigned)reg[rs2]);break;//BGEU
            }
        }
        else{
            switch(f3){
                case 0: result=(reg[rs2]&((1<<8)-1));break;//SB
                case 1: result=(reg[rs2]&((1<<16)-1));break;//SH
                case 2: result=reg[rs2];break;//SW
            }
        }
    }
    void MEM(){
        int a,b,c,d;
        if(opcode==99){
            switch(f3){
                case 0:
                case 1:
                case 4:
                case 5:
                case 6:
                case 7:break;
            }
        }
        else{
            switch(f3){
                case 0:memory[reg[rs1]+imm]=(char)result;break;
                case 1:a=(result)&((1<<8)-1);b=result>>8;
                       memory[reg[rs1]+imm]=(char)a;memory[reg[rs1]+imm+1]=(char)b;
                       break;
                case 2: a=(result)&((1<<8)-1);b=(result>>8)&((1<<8)-1);
                        c=(result>>16)&((1<<8)-1);d=result>>24;
                        memory[reg[rs1]+imm]=(char)a;memory[reg[rs1]+imm+1]=(char)b;
                        memory[reg[rs1]+imm+2]=(char)c;memory[reg[rs1]+imm+3]=(char)d;
                        break;
            }
        }
    }
    void WB(){
        if(opcode==99){
            switch(f3){
                case 0: PC=(result? PC+imm:PC+4)-4;break;
                case 1: PC=(result? PC+imm:PC+4)-4;break;
                case 4: PC=(result? PC+imm:PC+4)-4;break;
                case 5: PC=(result? PC+imm:PC+4)-4;break;
                case 6: PC=(result? PC+imm:PC+4)-4;break;
                case 7: PC=(result? PC+imm:PC+4)-4;break;
            }
        }
        else{
            switch(f3){
                case 0:
                case 1:
                case 2:break;
            }
        }
    }
};
class I:public inst{
public:
    int rs1,rs2,rd,imm,f3,f7,opcode,code;
    int result;
    void ID(){
        opcode=code&((1<<7)-1);
        rd=(code>>7)&((1<<5)-1);
        f3=(code>>12)&((1<<3)-1);
        rs1=(code>>15)&((1<<5)-1);
        imm=(code>>20);
        if(opcode==19&&f3==1||f3==5){
            rs2=imm&((1<<5)-1);
            f7=(imm>>5);
        }
        imm=signedExtend(imm,11);
    }
    void EX(){
        switch(opcode){
            case 3:
                switch(f3){
                    case 0:result=reg[rs1]+imm;break;//LB
                    case 1:result=reg[rs1]+imm;break;//LH
                    case 2:result=reg[rs1]+imm;break;//LW
                    case 4:result=reg[rs1]+imm;break;//LBU
                    case 5:result=reg[rs1]+imm;break;//LHU
                }
            case 103:break;//JALR
            case 19:
                switch(f3){
                    case 0:result=reg[rs1]+imm;break;//ADDI
                    case 2:result=(reg[rs1]<imm);break;//SLTI
                    case 3:result=((unsigned)reg[rs1]<(unsigned)imm);break;//SLTIU
                    case 4:result=reg[rs1]^imm;break;//XORI
                    case 6:result=reg[rs1]|imm;break;//ORI
                    case 7:result=reg[rs1]&imm;break;//ANDI

                    case 1:result=imm;break;//SLLI
                    case 5:result=imm;//SRLI&SRAI
                             break;
                }
        }
    }
    void MEM(){
        switch(opcode){
            case 3:
                switch(f3){
                    case 0:result=(int)memory[result];break;
                    case 1:result=(int)memory[result]+((int)memory[result+1]<<8);break;
                    case 2:result=(int)memory[result]+((int)memory[result+1]<<8)+
                            ((int)memory[result+2]<<16)+((int)memory[result+3]<<24);break;
                    case 4:result=(int)memory[result];break;
                    case 5:result=(int)memory[result]+((int)memory[result+1]<<8);break;
                }
            case 103:break;
            case 19:
                switch(f3){
                    case 0:
                    case 2:
                    case 3:
                    case 4:
                    case 6:
                    case 7:

                    case 1:break;
                    case 5:break;
                }
        }
    }
    void WB(){
        switch(opcode){
            case 3:
                if(rd!=0){
                switch(f3){
                    case 0:reg[rd]=signedExtend(result,8);break;
                    case 1:reg[rd]=signedExtend(result,16);break;
                    case 2:reg[rd]=result;break;
                    case 4:reg[rd]=result;break;
                    case 5:reg[rd]=result;break;
                }break;}
                else break;
            case 103:if(rd!=0)reg[rd]=PC;PC=(reg[rs1]+imm)&0xfffffffe;break;
            case 19:
                if(rd!=0){
                switch(f3){
                    case 0:
                    case 2:
                    case 3:
                    case 4:
                    case 6:
                    case 7:reg[rd]=result;break;

                    case 1:reg[rd]=(reg[rs1]<<result);break;
                    case 5:
                        if(f7==0) reg[rd]=((unsigned)reg[rs1]>>result);
                                else reg[rd]=(reg[rs1]>>result);break;
                    }
                }
        }
    }
};
#endif //RISCV_OPERATOR_H
