#ifndef JOY_H
#define JOY_H

#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <string>
#include <bitset>
#include <array>
#include <cmath>

#define ON true
#define OFF false
#define ANY 0xffff
#define TOTAL_BUTTON 17
#define TOTAL_AXIS 6
#define TYPE_FILTER 0x7f
#define RATE_8B_per_16B 127 / 32767
#define DEVICE_LIST_FILE_NAME "/proc/bus/input/devices"

using namespace std;

class GamePad{
protected:
    bool connection;
    int dev_fd;
    string device_file;
    struct js_event event;

    int leftY_num, rightY_num;

    bitset<TOTAL_BUTTON> button_data, pre_button_data;
    array<int8_t, TOTAL_AXIS> axis_data;

    bool find_device_file(string controller_name);
    virtual void data_process();

public:
    GamePad(string controller_name);
    bool is_connect();
    void set_Ystick_num(int left, int right);
    void load_data();
    bool is_ON(int button_number);
    bool is_OFF(int button_number);
    bool is_Pushed(int button_number);
    bool is_Rleased(int button_number);
    int get_stick_value(int stick_number);
};

#endif