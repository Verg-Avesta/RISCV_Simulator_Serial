#include <iostream>
#include<iomanip>
#include<bitset>
#include "operator.hpp"
#include "RISCV.hpp"
using namespace std;

/*const int maxn =0x3fffff;
char memory[maxn];//内存
int reg[32];//寄存器
int PC=0;//PC寄存器
int length=0;*/

void IF(int code){
    int opcode;
    opcode=code&((2<<6)-1);
    switch(opcode){
        case 55:
        case 23:
        case 111:U ins; ins.code=code;ins.ID();ins.EX();ins.MEM();ins.WB();/*cout<<"U"<<endl;*/break;
        case 51: R ins1; ins1.code=code;ins1.ID();ins1.EX();ins1.MEM();ins1.WB();/*cout<<"R"<<endl;*/break;
        case 35:
        case 99: S ins2; ins2.code=code;ins2.ID();ins2.EX();ins2.MEM();ins2.WB();/*cout<<"S"<<endl;*/break;
        case 3:
        case 19:
        case 103:I ins3;ins3.code=code;ins3.ID();ins3.EX();ins3.MEM();ins3.WB();/*cout<<"I"<<endl;*/break;
    }
}
int main() {
     unsigned char tmp[15];
     int pos=0,temp=0;
     while(cin>>tmp){
        if(tmp[0]=='@'){
            int i;
            for(i=1;i<9;i++){
                if(tmp[i]>='A'&&tmp[i]<='Z')
                    temp+=(tmp[i]-'A'+10)<<(4*(8-i));
                else
                    temp+=(tmp[i]-'0')<<(4*(8-i));
            }
            pos=temp;temp=0;continue;
        }

        if(tmp[0]>='A'&&tmp[0]<='Z')
            memory[pos]=(unsigned char)((tmp[0]-'A'+10)<<4);
        else
            memory[pos]=(unsigned char)((tmp[0]-'0')<<4);

        if(tmp[1]>='A'&&tmp[1]<='Z')
            memory[pos]+=(tmp[1]-'A'+10);
        else
            memory[pos]+=(tmp[1]-'0');
        pos++;
        length++;
        //cout<<memory[length-1];
    }//将数据输入到内存中

   /* for(int i=0;i<maxn;i++)if(memory[i]!=0)cout<<hex<<i<<':'<<(int)memory[i]<<' ';
    cout<<endl<<length<<endl;*/

    for(int i=0;i<32;i++)reg[i]=0;
    int code;
    int i=0;
    while(true){
        //cout<<i++<<' ';
        code=(int)((memory[PC+3]<<24)+(memory[PC+2]<<16)+(memory[PC+1]<<8)+memory[PC]);//cout<<hex<<PC<<' ';
        PC+=4;//cout<<bitset<32>(code)<<' ';
        //cout<<hex<<(int)memory[PC]<<(int)memory[PC+1]<<(int)memory[PC+2]<<(int)memory[PC+3]<<endl;

        IF(code);
        if(memory[0x30004]!=0) {cout<<((reg[10])&((1<<8)-1));return 0;}
    }
    for(int j=0;j<32;j++)cout<<reg[j]<<' ';
    return 0;
}
/**第0个寄存器始终为0，尝试改变x0的操作必须失败*/
