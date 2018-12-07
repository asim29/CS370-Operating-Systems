#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int bitExtracted(unsigned int number, int k, int p);
int hexToDec(char* num);
#define FRAME_SIZE 1024
#define DATA_FRAMES 95
#define L1_SIZE 64
#define L2_FRAMES 32
#define L2_SIZE 256

int main(){
    FILE *fp,*store;
    fp = fopen("addresses.txt","r");
    store = fopen("DATA_STORE.bin","r");
    unsigned int addr;

    int rw;
    int l2_FIFO = 0;
    int frame_FIFO = 0;
    int l1_table[L1_SIZE];
    int l2_table[L2_FRAMES][L2_SIZE];
    int memory[DATA_FRAMES][FRAME_SIZE];
    int page_faults = 0;
    int table_faults = 0;
    int count = 0;

    int l2_backing[L1_SIZE][L2_SIZE];

    // Initialize arrays:
    for(int i = 0; i < L1_SIZE; i++){
        l1_table[i] = -1;
    }

    for(int i = 0; i < L2_FRAMES; i++){
        for(int j = 0; j < L2_SIZE; j++){
            l2_table[i][j] = -1;
        }
    }

    for(int i = 0; i < L1_SIZE; i++){
        for(int j = 0; j < L2_SIZE; j++){
            l2_backing[i][j] = -1;
        }
    }

    while(!feof(fp)){
        count++;
        fscanf(fp,"%x",&addr);
        addr = bitExtracted(addr,24,1);
        fscanf(fp,"%d",&rw);
        unsigned int l2_index = bitExtracted(addr,6,19);
        unsigned int frame_index = bitExtracted(addr,8,11);
        unsigned int offset = bitExtracted(addr,10,1);

        int l2_pos = l1_table[l2_index];
        int value;
        int page_miss = 0;
        int table_miss = 0;
        // Case 1: Page 2 table not in memory:
        if(l2_pos == -1){
            table_faults++;

            page_miss = 1;
            // Replace table at L2_FIFO index with new table
            for(int i = 0; i < L2_SIZE; i++){
                l2_table[l2_FIFO][i] = l2_backing[l2_index][i];
            }

            for(int i = 0; i < L1_SIZE; i++){
                if(l1_table[i] == l2_FIFO)
                    l1_table[i] = -1;
            }

            l1_table[l2_index] = l2_FIFO;

            // Check if frame in memory or not 
            int frame_pos = l2_table[l2_FIFO][frame_index];

            if(frame_pos == -1){
                page_faults++;

                for(int i = 0; i < L2_SIZE; i++){
                    if(l2_table[l2_FIFO][i] == frame_FIFO)
                        l2_table[l2_FIFO][i] = -1;
                }

                l2_table[l2_FIFO][frame_index] = frame_FIFO;

                int data[FRAME_SIZE]; //READ DATA FROM BACKING STORE
                fseek(store,l2_index*256*64+frame_index*64,SEEK_SET);
                fread(&data,1,FRAME_SIZE,store);
                // Replace page at frame_FIFO index with new page
                for(int i = 0; i < FRAME_SIZE; i++){
                    memory[frame_FIFO][i] = data[i];
                }

                value = memory[frame_FIFO][offset];

                frame_FIFO = (frame_FIFO+1)%DATA_FRAMES;
            } else { 
                value = memory[frame_pos][offset];
            }
            
            l2_FIFO = (l2_FIFO+1)%L2_FRAMES;
        } else {
            int frame_pos = l2_table[l2_pos][frame_index];
            // Case 2: Page 2 table in memory BUT page not in page 2
            if(frame_pos == -1){
                page_faults++;
                for(int i = 0; i < L2_SIZE; i++){
                    if(l2_table[l2_pos][i] == frame_FIFO)
                        l2_table[l2_pos][i] = -1;
                }
                
                l2_table[l2_pos][frame_index] = frame_FIFO;

                int data[FRAME_SIZE]; //READ DATA FROM BACKING STORE
                fseek(store,l2_index*256*64+frame_index*64,SEEK_SET);
                fread(&data,1,FRAME_SIZE,store);    
                // Replace page at frame_FIFO index with new page
                for(int i = 0; i < FRAME_SIZE; i++){
                    memory[frame_FIFO][i] = data[i];
                }

                value = memory[frame_FIFO][offset];

                frame_FIFO = (frame_pos+1)%DATA_FRAMES;

            } else {
                value = memory[frame_pos][offset];

            }
        }
        


        printf("logical Address: 0x%X ", addr);
        printf("Outer Page Index : %d ", l2_index);
        printf("Inner Page Index : %d ", frame_index);
        printf("Value : 0x%X\n", value);
        if(table_miss)
            printf("  No  ");
        else
            printf("  Yes  ");

        if(page_miss)
            printf("  No  \n");
        else
            printf("  Yes  \n");

        // break;
    }

    printf("Percentage Table Misses: %f \n", (table_faults/100001.0)*100.0);
    printf("Table Misses: %d \n", table_faults);
    printf("Percentage Page Misses: %f \n", (page_faults/100001.0)*100.0);
    printf("Page Misses: %d \n", page_faults);
    printf("Count: %d\n", count);
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
