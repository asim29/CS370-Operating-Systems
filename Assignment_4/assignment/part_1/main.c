#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// FUNCTION HEADER //
void printArray(unsigned int* arr,int N);
void printBin(unsigned int n);
int bitExtracted(unsigned int number, int k, int p);
int hexToDec(char* num);
// ------------------------------ //


int main(){
    FILE* fp;
    unsigned int addr;
    int rw;

    fp = fopen("addresses.txt","r");
    int pageFaults = 0;
    char pageTable[256][2];
    char memory[62][256];
    int i,j;

    for(i=0;i<256;i++){
        pageTable[i][0] = '0';
    }

    while(!feof(fp)){
        fscanf(fp,"%x",&addr);
        printf("Logical Address: %x ", addr);
        fscanf(fp,"%d",&rw);
        printf(" Read/Write: %d ",rw);
        unsigned int last16 = bitExtracted(addr,16,1);
        unsigned int offset = bitExtracted(last16,8,1);
        unsigned int pageNo = bitExtracted(last16,8,9);
        printf("Page Number: %d ", pageNo);
        printf("Offset: %d \n", offset);

        if(pageTable[pageNo][0] == 'G' && pageTable[pageNo][1] == 'G'){
            pageFaults++;
            
        }

    }
    return 0;
}


// HELPER FUNCTIONS //
void printBin(unsigned int n){ 
    unsigned int displayMask = 1 << 31;
    unsigned int c;
    for(c =1; c<=32;c++){
        putchar(n & displayMask ? '1' : '0');
        n<<=1;

        if(c%8 == 0){
            putchar(' ');
        }
    }
    putchar('\n');
}

void printArray(unsigned int* arr, int N){
    int i;
    for(i=0;i<N;i++){
        printf("%d\n",arr[i]);
    }
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
        if(number[i]) >= '0' && number[i] <= '9'){
            dec += (base*(number[i] - 48));
            base*=16;
        }
        else if(number[i] >= 'A' && number[i] <= 'F'){
            dec+= (base*(number[i] - 55));
            base*=16;
        }
        return dec;
    }
}
// -------------------------------- //
