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
    char pageTable[256][2];
    char memory[62][256];
    int i,j;
    int lastIndex = 0;
    for(i=0;i<256;i++){
        pageTable[i][0] = 'G';
        pageTable[i][1] = 'G';
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
            (lastIndex++ % 62);
            //Get data from backign store
            fseek(store,pageNo*256,SEEK_SET);
            char data[256];
            fread(&data,1,256,store);
            printf("%s\n",data);
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
