#ifndef PS5_H
#define PS5_H

#include "joy.h"

#define CONTROLLER_NAME "\"DualSense Wireless Controller\""

// buttons
#define CROSS       0
#define CIRCLE      1
#define TRIANGLE    2
#define SQUARE      3
#define L1          4
#define R1          5
#define L2          6
#define R2          7
#define CREATE      8
#define OPTION      9
#define HOME        10
#define L3          11
#define R3          12
#define LEFT        13
#define RIGHT       14
#define UP          15
#define DOWN        16

// sticks
#define LEFT_X      0
#define LEFT_Y      1
#define L3_DEPTH    2
#define RIGHT_X     3
#define RIGHT_Y     4
#define R3_DEPTH    5

#define CROSS_X_AXIS 6
#define CROSS_Y_AXIS 7

class DualSense: public GamePad{
protected:
    void data_process();
public:
    DualSense(string controller_name);
    void get_stick_arg(double& left_arg, double& right_arg);
};

#endif