#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>

namespace pad{

    extern const std::string device_info_file;

    enum class Button_type{
        Any,
        None
    };

    struct Device_info{
        int device_fd;
        std::string device_file;
        std::string device_name;
        struct js_event event;
        bool connection;
    };

    class GamePad{
    protected:
        Device_info pad_info;
        const static std::string wired_controller_name;
        const static std::string wireless_controller_name;
        std::vector<bool> button_data, pre_button_data; 
        std::vector<int16_t> axis_data;

    protected:
        bool link_device_file(std::string device_name);

        void basic_button_process();
        void basic_axis_process(int axis_value);
        virtual void button_process() = 0;
        virtual void axis_process() = 0;
        void data_process();

    public:
        GamePad(std::string device_name);
        GamePad(std::string device_name, int total_button, int total_axis);
        virtual ~GamePad();

        bool is_connect();
        bool load_data();
        bool is_ON(int number);
        bool is_ON(Button_type type);
        bool is_Pushed(int number);
        bool is_Released(int number);
        int get_axis_value(int number);
    };

    inline void GamePad::basic_button_process(){
        button_data[pad_info.event.number] = pad_info.event.value;
    }

    inline void GamePad::basic_axis_process(int axis_value){
        axis_data[pad_info.event.number] = axis_value;
    }

}
#endif // GAMEPAD_H;