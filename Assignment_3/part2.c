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
int** getting_out;
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
	getting_out = (int**) malloc(numStations*sizeof(int*));
	move = (sem_t*) malloc(maxStations*sizeof(sem_t));

	sem_init(&mutexC, 0, 1);
	for(i = 0; i < NUM_TRAINS; i++){
		trains[i] = i;
		sem_init(&train_cap[i], 0, maxNumPeople);
		getting_out[i] = (int*) malloc(maxStations*sizeof(int));
	}

	for(i = 0; i < NUM_TRAINS; i++){
		int j;
		for(j = 0; j < maxStations; j++){
			getting_out[i][j] = 0;
			// printf("%d, %d\n", i, j);	
		}
	}
	for(i = 0; i < numStations; i++){
		getting_in[i] = 0;
		sem_init(&move[i], 0, 1);
	}
	// printf("Initialization complete\n");
}
void check_train(int stat, int train){
	// printf("In check train\n");
	// sleep(1);
	while(getting_in[stat]){

	}
	while(getting_out[train][stat]){

	}
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
		// printf("Train %d is at %d\n", id, trains[id]);
		check_train(trains[id], id);
		// printf("Telling %d to wait\n", (pos+1)%maxStations);
		sem_wait(&move[(pos+1)%maxStations]);
		int post_pos;
		if(pos == 0){
			post_pos = maxStations-1;
		} else{
			post_pos = pos-1;
		}
		// printf("Telling %d to post\n", (post_pos)%maxStations);
		sem_post(&move[(post_pos)]);

		pos = (pos+1)%maxStations;
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
    getting_in[arg->from]++;
	for(i = 0; i < NUM_TRAINS; i++){
		if(trains[i] == arg->from){
			sem_wait(&train_cap[i]);
			the_train = i;
			getting_in[arg->from]--;
			getting_out[the_train][arg->to]++;
			// printf("User %d has gotten into train %d to get out at %d\n", arg->id, the_train, arg->to);
			break;
		}
	}
	while(trains[the_train] != arg->to){
	
	}
	getting_out[the_train][arg->to]--;
	sem_post(&train_cap[i]);
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
