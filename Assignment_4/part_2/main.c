#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int bitExtracted(unsigned int number, int k, int p);
int hexToDec(char* num);

int main(){
    FILE *fp,*store;
    fp = fopen("addresses.txt","r");
    store = fopen("BACKING_STORE.bin","r");
    unsigned int addr;
    int rw;
    while(!feof(fp)){
        fscanf(fp,"%x",&addr);
        addr = bitExtracted(addr,24,1);
        fscanf(fp,"%d",&rw);
        printf("Logical Address: 0x%X ", addr);
        unsigned int outerPageIndex = bitExtracted(addr,6,1);
        unsigned int innerPageIndex = bitExtracted(addr,8,7);
        unsigned int offset = bitExtracted(addr,10,8);
        printf("Outer Page Index : %d ", outerPageIndex);
        printf("Inner Page Index : %d ", innerPageIndex);
        printf("Offset : %d\n", offset);
        break;
    }
    return 0;
}



int bitExtracted(unsigned int number, int k, int p) { 
     return (((1 << k) - 1) & (number >> (p - 1)));
} 

int hexToDec(char* num){
    int len = strlen(num);
    int base = 1;
    int dec = 0;
    int i;

    for(i=len-1;i>=0;i--){
        if(num[i] >= '0' && num[i] <= '9'){
            dec += (base*(num[i] - 48));
            base*=16;
        }
        else if(num[i] >= 'A' && num[i] <= 'F'){
            dec+= (base*(num[i] - 55));
            base*=16;
        }
        return dec;
    }
}