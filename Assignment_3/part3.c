#include <pthread.h>
#include "main.h"
#include "part3.h"
#include <stdio.h>
#include <semaphore.h>

// North = 0
// South = 1
// East = 2
// West = 3
// Right = 1
// Left = 0
// RED = 1
// GREEN = 0

int** lanes;
int* light;
sem_t mutexLanes;
sem_t *switchLight;

char* getLight(int i){
	char *trafficLight[] = {"NORTH","SOUTH","EAST","WEST"};
	return trafficLight[i];
}
char* getLane(int i){
	char *lane[] = {"LEFT","RIGHT"};
	return lane[i];
}
void initializeP3() {
	lanes = (int**) malloc(4*sizeof(int*));
	light = (int*) malloc(4*sizeof(int));
	switchLight = (sem_t*) malloc(4*sizeof(int));

	sem_init(&mutexLanes, 0, 1);

	int i;
	for(i = 0; i < 4; i++){
		lanes[i] = (int*) malloc(2*sizeof(int));
		light[i] = 1;
		sem_init(&switchLight[i], 0, 0);
	}
	for(i = 0; i < 4; i++){
		int j;
		for(j = 0; j < 2; j++){
			lanes[i][j] = 0;
		}
	}
}

void check_light(int l){
	int c = 5;
	while((lanes[l][0] || lanes[l][1]) && c){
		c--;
	}
}

// Going Straight = 1
// Turning left = 0
// Turning right = 2
int get_direction(int from, int to){
	if(from == 0){
		if(to == 1){
			return 1; 
		} else if(to == 2){
			return 0;
		} else if(to == 3){
			return 2;
		}
	} else if(from == 1){
		if(to == 0){
			return 1;
		} else if(to == 2){
			return 2;
		} else if(to == 3){
			return 0;
		}
	} else if(from == 2){
		if(to == 0){
			return 2;
		} else if(to == 1){
			return 0;
		} else if(to == 3){
			return 1;
		}
	} else if(from == 3){
		if(to == 0){
			return 0;
		} else if(to == 1){
			return 2;
		} else if(to == 2){
			return 1;
		}
	}
}

void* traffic_light(void *light_arg){
	int* light_pointer = (int*) light_arg;
	int light_id = *light_pointer;

	sleep(1);
	int next;
	int prev;
	if(light_id == 0){
		next = 2;
		prev = 3;
	} else if(light_id == 1){
		next = 3;
		prev = 2;
	} else if(light == 2){
		next = 1;
		prev = 0;
	} else if(light == 3){
		next = 0;
		prev = 1;
	}

	if(light_id == 0){
		light[light_id] = 0;
		check_light(light_id);
		light[light_id] = 1;
		sem_post(&switchLight[next]);
	}
	
	while(1){
		sem_wait(&switchLight[prev]);
		light[light_id] = 0;
		check_light(light_id);
		light[light_id] = 1;
		sem_post(&switchLight[next]);
	}


}
/**
 * If there is a car going from SOUTH to NORTH, from lane LEFT,
 * print 
 * SOUTH NORTH LEFT
 * Also, if two cars can simulateneously travel in the two lanes,
 * first print all the cars in the LEFT lane, followed by all the
 * cars in the right lane
 */
void* goingFromToP3(void *argu){
	struct argumentP3 *argz = (struct argumentP3*) argu;
	printf("from %d, to %d, lane %d, id %d\n", argz->from, argz->to, argz->lane, argz->user_id ); 
	
	int direction;
	direction = get_direction(argz->from, argz->to);
	if(direction == 0){
		if(lanes[argz->from][0] == 0){
			printf("%s %s %s %d\n", getLight(argz->from), getLight(argz->to), getLane(argz->lane), argz->user_id ); 
			return;
		}
	}
	// sem_wait(&mutexLanes);
	lanes[argz->from][argz->lane]++;
	// sem_post(&mutexLanes);
	while(lanes[argz->from][argz->lane] > 1){

	}

	if(direction == 0){
		sem_wait(&mutexLanes);
		lanes[argz->from][argz->lane]--;
		sem_post(&mutexLanes);
		printf("%s %s %s %d\n", getLight(argz->from), getLight(argz->to), getLane(argz->lane), argz->user_id ); 
		return;
	}
	// while light[from] == 1 (light is red)
	// stay in infinite loop and wait until light becomes 0
	while(light[argz->from]){

	}
	// sem_wait(&mutexLanes);
	lanes[argz->from][argz->lane]--;
	// sem_post(&mutexLanes);
	printf("%s %s %s %d\n", getLight(argz->from), getLight(argz->to), getLane(argz->lane), argz->user_id ); 

}

void startP3(){	
	int c;
	pthread_t* traffic_tid;
	int *nums = (int*) malloc(4*sizeof(int));
	for(c = 0; c < 4; c++){
		nums[c] = c;
	}

	traffic_tid = (pthread_t*) malloc(4*sizeof(pthread_t));
	for(c = 0; c < 4; c++){
		pthread_create(&traffic_tid[c], NULL, traffic_light, &nums[c]);
	}
}
