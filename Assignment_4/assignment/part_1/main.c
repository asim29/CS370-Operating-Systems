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
    FILE* fp,*store;
    unsigned int addr;
    int rw;

    fp = fopen("addresses.txt","r");
    store = fopen("BACKING_STORE.bin","r");
    int pageFaults = 0;
    float hits = 0.0;
    int totalRefs = 0;

    unsigned int pageTable[256];
    char memory[62][256];
    int i,j;
    int lastIndex = 0;
    for(i=0;i<256;i++){
        pageTable[i] = -1;
    }

    while(!feof(fp)){
        fscanf(fp,"%x",&addr);
        addr = addr&0xFFFF;
        fscanf(fp,"%d",&rw);
        unsigned int last16 = bitExtracted(addr,16,1);
        unsigned int offset = bitExtracted(last16,8,1);
        unsigned int pageNo = bitExtracted(last16,8,9);
        totalRefs++;
        if(pageTable[pageNo] == -1){
            pageFaults++;            //Get data from backing store
            fseek(store,pageNo*256,SEEK_SET);
            char data[256];
            fread(&data,1,256,store);
            
            for(size_t i=0; i<256;i++){
                memory[lastIndex][i] = data[i];
            }

            for(size_t i = 0; i < 256;i++){
                if(pageTable[i] == lastIndex){
                    pageTable[i] = -1;
                }
            }

            pageTable[pageNo] = lastIndex;
            lastIndex = (lastIndex+1) % 62;
            int value = memory[pageTable[pageNo]][offset];
            value >= 0xFFFFFF00 ? value-=0xFFFFFF00 : value;

            printf("Logical Address: 0x%X ", addr);
            printf("Phsyical Address: 0x%X%X ", pageTable[pageNo],offset);
            printf("Value: 0x%X ", value);
            // printf(" Read/Write: %d ",rw);
            // printf("Page Number: %d ", pageNo);
            // printf("Offset: %d \n", offset);
            printf("Page Fault: True \n");
        }
        else{
            hits++;
            int value = memory[pageTable[pageNo]][offset];
            value >= 0xFFFFFF00 ? value-=0xFFFFFF00 : value;
            printf("Logical Address: 0x%X ", addr);
            printf("Phsyical Address: 0x%X%X ", pageTable[pageNo],offset);            
            // printf(" Read/Write: %d ",rw);
            // printf("Page Number: %d ", pageNo);
            // printf("Offset: %d \n", offset);
            printf("Value: 0x%X ", value);
            printf("Page Fault: False \n");
        }
    }
    printf("Total Page Faults: %d\n", pageFaults);
    printf("Hit rate: %f\n",hits/totalRefs * 100.0);
    printf("Fault rate: %f\n",100.0 - hits/totalRefs * 100.0);
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
// -------------------------------- //
