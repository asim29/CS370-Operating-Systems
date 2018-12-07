/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define MAX_LINE		80 /* 80 chars per line, per command */
#define HIST_SIZE		5
int parse(char* line, char** args){
	char* p = strtok(line, " ");
	int i = 0;
	while(p != NULL){
		args[i] = p;
		i++;
		p = strtok(NULL," ");
	}
	args[i] = NULL;
	return i;
}

int checkAndRemoveAmpersand(char ** args, int numArgs){
	char* c = args[numArgs-1];
	int conc = 0;
	
	if(!strcmp(c,"&\n")){
	conc = 1;
	args[numArgs-1] = NULL;
	}
	else{
		while(*c != '\n'){
			c++;
		}
		*c = NULL;
	}
	return conc;
}

void removeNewlineChar(char *line){
	char* ptr = line;

	while(*ptr != '\n'){
		ptr++;
	}
	*ptr = NULL;
}

void insert(char **arr, int *rear, int *count, char *line){
	if (*rear < HIST_SIZE){
		arr[*rear] = strdup(line);
		(*rear)++;
	} else {
		int i;
		for(i = 1; i < HIST_SIZE; i++){
			arr[i-1] = arr[i];
		}
		arr[*rear-1] = strdup(line);
	}
	(*count)++;
}

void print_full(char **arr){
	fflush(stdout);
	printf("History is:\n");
	int i = 0;
	for(i = 0; i < HIST_SIZE; i++){
		printf(arr[i]);
		fflush(stdout);	
	}
	printf("\n");
	fflush(stdout);
}

char* find_task(char **arr, int N, int task_count){
	if(N > task_count){
		return "-1";		
	}
	if(task_count < HIST_SIZE){
		return arr[N-1];
	} else {
		int overflow = task_count - HIST_SIZE;
		int i = N - overflow - 1;
		// printf("task_count: %d:",task_count);
		// printf("overflow: %d:",overflow);
		// printf("i: %d:",i);
		
		if(i < 0){
			return "-1";
		} else {
			// printf("real index is: %d\n", i);
			return arr[i];
		}
	}
	printf("\n");
	fflush(stdout);		

}


int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	char line[MAX_LINE];
	int should_run = 1;
	pid_t pid; 

	// Make array of strings of the max defined history size
	char *hist[HIST_SIZE];
	int rear = 0; // to define where the end of the array is
	int task_count = 0;

	int i = 0;
	for(i = 0; i < HIST_SIZE; i++){
		hist[i] = NULL;
	}
	i = 0;


	// print_full(hist);
	// printf("%d\n", rear);
	// printf("%d\n", task_count);
	// fflush(stdout);


	// insert(hist, &rear, &task_count, "hello how are you 6\n");
	// print_full(hist);
	// printf("%d\n", rear);
	// printf("%d\n", task_count);
	// fflush(stdout);
	// print_single(hist, 4, task_count);

	while(should_run){
		printf("\nosh>");
		fflush(stdout);
		fgets(line,MAX_LINE,stdin);
		if(!strcmp(line,"")){
			continue;
        }


		int numArgs = parse(strdup(line),args);
		int conc = checkAndRemoveAmpersand(args,numArgs);
		int status;

		if(!strcmp(args[0], "\n")){
			continue;
		}
        else if(!strcmp(args[0], "exit")){
        	// printf("wow");
        	fflush(stdout);
        	should_run = 0;
        	exit(0);
        } else if (!strcmp(args[0], "history")) {
        	print_full(hist);
        } else if(args[0][0] == '!'){

        	if(args[0][1] == '!'){
				// printf("Line is: ");
				// printf(line);
        		char *hist_line = strdup(hist[rear-1]);

        		// printf("Line from history: ");
        		// printf(hist_line);
        		// fflush(stdout);

				int numArgs = parse(strdup(hist_line),args);
				int conc = checkAndRemoveAmpersand(args,numArgs);
				int status;
				
				pid = fork();

		        if(pid == 0){
		        	execvp(args[0], args);
		        } 
		        else if( pid > 0){
		        	// Parent
		        	if(conc){
		        		// sleep(1);
		        		continue;		

		        	}
		        	else{
		        		while(wait(&status) != pid);
		        	}
		        }
		        else {
		        	printf("Child process creation failed...\n");
		        	fflush(stdout);

		        }
        	} else {
        		char *theline[MAX_LINE];
				strcpy(theline, args[0]);
        		char *num = theline;
        		num++;
        		int N = atoi(num);
        		// printf("%d\n",N);

        		char* hist_line =  strdup(find_task(hist, N, task_count));

        		if(!strcmp(hist_line,"-1")){
					char result[] = "Command not in history \n";
        			printf(result);
        			continue;
        		}

        		printf("Line from history: ");
        		printf(hist_line);
        		fflush(stdout);

				int numArgs = parse(strdup(hist_line),args);
				int conc = checkAndRemoveAmpersand(args,numArgs);
				int status;
				
				pid = fork();

		        if(pid == 0){
		        	execvp(args[0], args);
		        } 
		        else if( pid > 0){
		        	// Parent
		        	if(conc){
		        		// sleep(1);
		        		continue;		

		        	}
		        	else{
		        		while(wait(&status) != pid);
		        	}
		        }
		        else {
		        	printf("Child process creation failed...\n");
		        	fflush(stdout);

		        }
        	}
        }
        else{
			// printf("Line is: ");
			// printf(line);
			insert(hist, &rear, &task_count, line);
			// print_full(hist);
	        
	        pid = fork();

	        if(pid == 0){
	        	execvp(args[0], args);
	        } 
	        else if( pid > 0){
	        	// Parent
	        	if(conc){
	        		// sleep(1);
	        		continue;		

	        	}
	        	else{
	        		while(wait(&status) != pid);
	        	}
	        }
	        else {
	        	printf("Child process creation failed...\n");
	        	fflush(stdout);

	        }
        }
        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */
    }
    
	return 0;
}
