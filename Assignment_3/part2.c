#include <pthread.h>
#include "main.h"
#include "part2.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

const int INTER_ARRIVAL_TIME = 5;
const int NUM_TRAINS = 5;

int maxPeople;
int maxStations;
int* trains;
int* getting_in;
int* getting_out;
sem_t* train_cap;
sem_t* move;
sem_t mutexC;
sem_t* train_made;

/**
 * Do any initial setup work in this function.
 * numStations: Total number of stations. Will be >= 5. Assume that initially
 * the first train is at station 1, the second at 2 and so on.
 * maxNumPeople: The maximum number of people in a train
 */
void initializeP2(int numStations, int maxNumPeople) {
	maxPeople = maxNumPeople;
	maxStations = numStations;
	int i;
	trains = (int*) malloc(NUM_TRAINS*sizeof(int*));
	train_cap = (sem_t*) malloc(NUM_TRAINS*sizeof(sem_t));
	getting_in = (int*) malloc(numStations*sizeof(int));
	getting_out = (int*) malloc(numStations*sizeof(int));
	move = (sem_t*) malloc(maxStations*sizeof(sem_t));

	sem_init(&mutexC, 0, 1);
	for(i = 0; i < NUM_TRAINS; i++){
		trains[i] = i;
		sem_init(&train_cap[i], 0, maxNumPeople);
	}

	for(i = 0; i < numStations; i++){
		getting_in[i] = 0;
		getting_out[i] = 0;
		sem_init(&move[i], 0, 1);
	}
	// printf("Initialization complete\n");
}
void check_train(){
	// printf("In check train\n");
	sleep(1);
	// printf("exiting check train\n");
}
void* train(void* pos_arg){
	sem_wait(&mutexC);
	int *pos_point = (int *) pos_arg;
	int pos = *pos_point;
	int id = pos;
	// printf("Train made with id %d\n", *pos_point);
	sem_post(&mutexC);
	sleep(1);
	// printf("loop starting\n");
	fflush(stdout);

	while(1){
		check_train();
		sem_wait(&move[(pos+1)%maxStations]);
		sem_post(&move[(pos-1)%maxStations]);		
		pos++;
		trains[id] = pos;
	}

}

/**
 * Print in the following format:
 * If a user borads on train 0, from station 0 to station 1, and another boards
 * train 2 from station 2 to station 4, then the output will be
 * 0 0 1
 * 2 2 4
 */
void *goingFromToP2(void *argu) {
	int the_train;
    struct argument *arg = (struct argument*)(argu);
    int i;
	for(i = 0; i < NUM_TRAINS; i++){
		if(trains[i] == arg->from){
			the_train = i;
		}
	}
	while(trains[the_train] != arg->to){

	}
	printf("%d %d %d\n", arg->id, arg->from, arg->to);

}

void startP2(){
	int c;
	pthread_t* train_tid;

	train_tid = (pthread_t*) malloc(NUM_TRAINS*sizeof(pthread_t));
	for(c = 0; c < NUM_TRAINS; c++){
		// printf("c is: %d\n", c);
		pthread_create(&train_tid[c], NULL, train, &trains[c]);
		// printf("thread %d created\n", c);
		// sleep(1);
	}

}
