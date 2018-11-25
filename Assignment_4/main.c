#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// FUNCTION HEADER //
char* readAddr(char* filename);
char** str_split(char* a_str, const char a_delim);
unsigned int castInt(char *st);
unsigned int* convert(char** str);
void printArray(unsigned int* arr);
void printBin(unsigned int n);
// ------------------------------ //

#define N 6 // ENTER NUMBER OF INPUT ADDRESSES HERE

int main(){
    char* file_name = "logical_addresses.txt";

    printf("The contents of %s file are:\n", file_name);
    
    char* fileData = readAddr(file_name);
    
    char** strData;
    unsigned int* data;

    if (fileData){
        strData = str_split(fileData,','); // Split Input data;
        data = convert(strData); // Convert Addresses to unsigned int
        printArray(data);
        int i;
        for(i=1;i<2;i++){
            unsigned int lsbHalf;
            unsigned int pageNum;
            unsigned int offset;
            lsbHalf = data[i] & ((1 << 16) - 1);
            pageNum = lsbHalf & ((1 << 8));
            printf("%d : ", data[i]);
            printBin(data[i]);
            
            printf("LSB HALF of %d : ", data[i]);
            printBin(lsbHalf);
            
            printf("Page Number of %d : ", data[i]);
            printBin(pageNum);
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

void printArray(unsigned int* arr){
    int i;
    for(i=0;i<N;i++){
        printf("%d\n",arr[i]);
    }
}

unsigned int* convert(char** str){
    int i;
    unsigned int* data = (unsigned int*) malloc(N * sizeof(unsigned int));

    for(i=0;i<N;i++){
        unsigned int temp = castInt(str[i]);
        data[i] = temp;
    }

    return data;
}

unsigned int castInt(char *st) {
  char *x;
  for (x = st ; *x ; x++) {
    if (!isdigit(*x))
      return 0L;
  }
  return (strtoul(st, 0L, 10));
}

char* readAddr(char* filename){
    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    if (f){
        fseek(f, 0, SEEK_END);
        length = ftell (f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length+2);
        if (buffer){
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }
    buffer[length] = '\0';
    buffer[length+1] = 'e';
    return buffer;
}


char** str_split(char* a_str, const char a_delim){
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            // assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        // assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
// -------------------------------- //