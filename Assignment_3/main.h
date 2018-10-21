#ifndef MAIN_H
#define MAIN_H

#include "part3.h"

struct argument {
    int from;
    int to;
    int id; //user id for part 1. train id for part 2
};

struct argumentP3 {
    enum DIRECTION from;
    enum DIRECTION to;
    enum LANE lane;
    int user_id;
};

#endif
