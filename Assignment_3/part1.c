#include "part1.h"
#include <pthread.h>
#include "main.h"
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>


/**
 * Do any initial setup work in this function.
 * numFloors: Total number of floors elevator can go to
 * maxNumPeople: The maximum capacity of the elevator
 */
int currentFloor;
int maxFloor;
int maxCapacity;
int* destination;
int* floor_in;
sem_t capacity;
// sem_t mutexFloor;
sem_t mutex_floor_in;
sem_t mutex_dest;
// sem_t* door;

// pthread_cond_t *floorCond;

void check(int i){
	sleep(1);
	// printf("In check on floor %d\nfloor[i]: %d \ndest[i]: %d \n", i, floor_in[i], destination[i]);
	// while(floor_in[i]){

	// }
	// while(destination[i]){

	// }
}

void* elevator(void* arg){
	sleep(1);
	while(1){
		// sem_wait(&mutexFloor);
		currentFloor = 0;
		// sem_post(&mutexFloor);
		// printf("FIRST currentFloor: %d\n", currentFloor);
		while(currentFloor < maxFloor){
			
			// printf("Before check: %d\n", currentFloor);
			// sem_post(&door[currentFloor]);
			// printf("Posted: %d\n", currentFloor);
			// printf("floor_in: %d\n", floor_in[currentFloor]);
			// printf("destination: %d\n", destination[currentFloor]);
			// while(floor_in[currentFloor] != -1 || destination[currentFloor] != -1){
				// printf("In while\n");

			// }
			check(currentFloor);
			// printf("After check: %d\n", currentFloor);
			// pthread_cond_signal(&floorCond[currentFloor]);
			// sem_wait(&mutexFloor);
			// sleep(0.5);
			// sleep(0.5);
			currentFloor++;
			// sem_post(&mutexFloor);
		}
		while(currentFloor >= 0){
			// printf("%d\n", currentFloor);
			check(currentFloor);
			// printf("Before check: %d\n", currentFloor);
			// sem_post(&door[currentFloor]);
			// printf("After check: %d\n", currentFloor);
			// pthread_cond_signal(&floorCond[currentFloor]);
			// sem_wait(&mutexFloor);
			currentFloor--;
			// sem_post(&mutexFloor);
		}
	break;
	}
}

void initializeP1(int numFloors, int maxNumPeople) {
	maxFloor = numFloors-1;
	maxCapacity = maxNumPeople;
	currentFloor = -1;
	destination = (int*) malloc(maxNumPeople * sizeof(int));
	floor_in = (int*) malloc(maxNumPeople * sizeof(int));
	// door = (sem_t*) malloc(numFloors * sizeof(sem_t));
	// floorCond = (pthread_cond_t*) malloc(numFloors * sizeof(pthread_cond_t));
	int i = 0;
	for(i = 0; i < maxFloor; i++){
		destination[i] = 0;
		floor_in[i] = 0;
		// sem_init(&door[i], 0, 0);
	}
	sem_init(&capacity, 0, maxCapacity);
	// sem_init(&mutexFloor, 0, 1);
	sem_init(&mutex_floor_in, 0, 1);
	sem_init(&mutex_dest, 0, 1);

}

/**
 * This program should print data in the specific format
 * described below:
 * If there is a passenger (id 5) going from floor 2 to 4, followed
 * by a passenger (id 6) from 2 to 5, the output should be:
 * 5 2 4
 * 6 2 5
 * i.e. there should be a single space, and each journey should be 
 * on a new line. Incorrectly formatted output will get 0 marks
 */
void* goingFromToP1(void *arg) {
    struct argument *temp = (struct argument *) arg;
	sem_wait(&capacity);
	
	
	// sem_wait(&mutex_floor_in);
	floor_in[temp->from]++;
	// sem_post(&mutex_floor_in);
	// printf("Thread made: id %d %d %d\n", temp->id, temp->from, temp->to);
	// sem_wait(&door[temp->from]);

	// printf("%d has got in\n", temp->id);
	// sem_wait(&door[temp->to]);
	while(1){
		// printf("in while");
		// sem_wait(&mutexFloor);
		// printf("After sen");
		if (currentFloor == temp->from){
			// sem_post(&mutexFloor);
			break;
		}
	}

	// sem_wait(&mutexFloor);
	// sem_wait(&mutex_floor_in);
	floor_in[temp->from]--;
	// printf("%d I got in!\n", temp->id);
	// sem_post(&mutex_floor_in);
	// sem_wait(&mutex_dest);
	destination[temp->to]++;
	// sem_post(&mutex_dest);
	// sem_post(&mutexFloor);

	while(1){
	// 	// sem_wait(&mutexFloor);
		if (currentFloor == temp->to){
			// sem_post(&mutexFloor);
			break;
		}
	}
	sem_wait(&mutex_dest);
	// printf("%d I got out!\n", temp->id);
	destination[temp->to]--;
	sem_post(&mutex_dest);
	// printf("%d %d %d\n", 0, 0, 0);
	printf("%d %d %d\n", temp->id, temp->from, temp->to);
	
	sem_post(&capacity);
	return NULL;

}

void startP1(){
	pthread_t elev;
	pthread_create(&elev, NULL, elevator, NULL);
}
	