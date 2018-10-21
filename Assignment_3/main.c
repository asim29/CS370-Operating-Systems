#include "main.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <assert.h>
#include "part1.h"
#include "part2.h"
#include "part3.h"


void catch_stdout(int pipefd[2], int *stdout_bk) {
    pipe2(pipefd, 0);
    *stdout_bk = dup(fileno(stdout));
    dup2(pipefd[1], fileno(stdout));
}

void restore_output(int pipefd[2], int stdout_bk) {
    fflush(stdout);
    close(pipefd[1]);
    dup2(stdout_bk, fileno(stdout));
}

void test(char *path) {
    FILE *test;
    int NUM_PESSENGERS;
    int NUM_FLOORS;
    int MAX_NUM_PEOPLE;
    int i = 0;
    int pipefd[2];
    int stdout_bk;

    catch_stdout(pipefd, &stdout_bk);
    test = fopen(path, "r");


    fscanf(test, "%d %d %d", &NUM_PESSENGERS, &NUM_FLOORS, &MAX_NUM_PEOPLE);
    int pessengers[NUM_PESSENGERS][2]; 

    initializeP1(NUM_FLOORS, MAX_NUM_PEOPLE);

    for(i = 0; i < NUM_PESSENGERS; i++) {
        fscanf(test, "%d %d", &pessengers[i][0], &pessengers[i][1]);
    }

    pthread_t threads[NUM_PESSENGERS];
    struct argument *args[5]; 
    for(i = 0; i < NUM_PESSENGERS; i++) {
        pthread_t tid;
        struct argument *arg = (struct argument*)malloc(sizeof(struct argument));
        arg->from = pessengers[i][0];
        arg->to = pessengers[i][1];
        arg->id = i; //train id or user id
        pthread_create(&tid, NULL, goingFromToP1, (void*) arg);
        args[i] = arg;
        threads[i] = tid;
    }

    startP1();

    for(i = 0; i < NUM_PESSENGERS; i++) {
        pthread_join(threads[i], NULL);
    }

    restore_output(pipefd, stdout_bk);
    FILE *temp = fdopen(pipefd[0],"r");
    for(i = 0; i < NUM_PESSENGERS; i++) {
        int id, from, to, test_id, test_from, test_to;
        fscanf(temp, "%d %d %d", &id, &from, &to);
        fscanf(test, "%d %d %d", &test_id, &test_from, &test_to);
        if(from != test_from || to != test_to) {
            printf("'%s' FAILED\n", path);
            return;
        }
    }

    printf("'%s' PASSED\n", path);
}

void test2(char *path) {
    FILE *test;
    int NUM_PESSENGERS;
    int NUM_STATIONS;
    int MAX_NUM_PEOPLE;
    int i = 0;
    int pipefd[2];
    int stdout_bk;

    catch_stdout(pipefd, &stdout_bk);
    test = fopen(path, "r");


    fscanf(test, "%d %d %d", &NUM_PESSENGERS, &NUM_STATIONS, &MAX_NUM_PEOPLE);
    int pessengers[NUM_PESSENGERS][2]; 

    initializeP2(NUM_STATIONS, MAX_NUM_PEOPLE);

    for(i = 0; i < NUM_PESSENGERS; i++) {
        fscanf(test, "%d %d", &pessengers[i][0], &pessengers[i][1]);
    }

    pthread_t threads[NUM_PESSENGERS];
    struct argument *args[5]; 
    for(i = 0; i < NUM_PESSENGERS; i++) {
        pthread_t tid;
        struct argument *arg = (struct argument*)malloc(sizeof(struct argument));
        arg->from = pessengers[i][0];
        arg->to = pessengers[i][1];
        arg->id = i;
        pthread_create(&tid, NULL, goingFromToP2, (void*) arg);
        args[i] = arg;
        threads[i] = tid;
    }

    startP2();

    for(i = 0; i < NUM_PESSENGERS; i++) {
        pthread_join(threads[i], NULL);
    }

    restore_output(pipefd, stdout_bk);
    int testData[NUM_PESSENGERS][3];
    FILE *temp = fdopen(pipefd[0],"r");
    int j;

    for(i = 0; i < NUM_PESSENGERS; i++) {
        int id, from, to;
        fscanf(test, "%d %d %d", &testData[i][0], &testData[i][1], &testData[i][2]);
    }

    for(i = 0; i < NUM_PESSENGERS; i++) {
        int id, from, to;
        fscanf(temp, "%d %d %d", &id, &from, &to);       
        int found = 0;
        for(j = 0; j < NUM_PESSENGERS; j++) {
            if (testData[j][0] == id && testData[j][1] == from && testData[j][2] == to)
                found = 1;
        }

        if(!found) {
            printf("'%s' FAILED\n", path);
            return;
        }
    }

    printf("'%s' PASSED\n", path);

}

int getEnum(char *val) {
    char *directions[] = {"NORTH", "SOUTH", "EAST", "WEST"};    
    char *lanes[] = {"LEFT", "RIGHT"};
    int i = 0;
    for(i = 0; i < 4; i++)
        if(!strcmp(val, directions[i]))
            return i;
    for(i = 0; i < 2; i++)
        if(!strcmp(val, lanes[i]))
            return i;
    return -1;
}
void test3(char *path) {
    FILE *test;
    int NUM_CARS;
    int i = 0;
    int j = 0;
    int pipefd[2];
    int stdout_bk;

    catch_stdout(pipefd, &stdout_bk);
    test = fopen(path, "r");

    fscanf(test, "%d", &NUM_CARS);
    char cars[NUM_CARS][3][6]; 
    initializeP3();

    for(i = 0; i < NUM_CARS; i++) {
        fscanf(test, "%s %s %s", cars[i][0], cars[i][1], cars[i][2]);
    }

    pthread_t threads[NUM_CARS];
    for(i = 0; i < NUM_CARS; i++) {
        pthread_t tid;
        struct argumentP3 *arg = (struct argumentP3*)malloc(sizeof(struct argumentP3));
        arg->from = getEnum(cars[i][0]);
        arg->to = getEnum(cars[i][1]);
        arg->lane = getEnum(cars[i][2]);
        arg->user_id = i;
        pthread_create(&tid, NULL, goingFromToP3, (void*) arg);
        threads[i] = tid;
    }

    startP3();

    for(i = 0; i < NUM_CARS; i++) {
        pthread_join(threads[i], NULL);
    }

    restore_output(pipefd, stdout_bk);
    char testData[NUM_CARS][3][6];
    FILE *temp = fdopen(pipefd[0],"r");

    for(i = 0; i < NUM_CARS; i++) {
        int id, from, to;
        fscanf(test, "%s %s %s", testData[i][0], testData[i][1], testData[i][2]);
        //printf("%s %s %s here\n", testData[i][0], testData[i][1], testData[i][2]);
    }

    for(i = 0; i < NUM_CARS; i++) {
        char answers[3][6];
        fscanf(temp, "%s %s %s", answers[0], answers[1], answers[2]);       
        if(strcmp(answers[0], testData[i][0]) || strcmp(answers[1], testData[i][1]) || strcmp(answers[2], testData[i][2])) {
            printf("'%s' FAILED\n", path);
            return;
        }
    }

    printf("'%s' PASSED\n", path);
}

int main() {
    test("test1"); 
    fflush(stdout);
    test2("test2");
    // test3("test3");
}
