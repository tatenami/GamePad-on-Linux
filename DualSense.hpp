#ifndef DUAL_SENSE_H
#define DUAL_SENSE_H

#include "GamePad.hpp"

namespace ds5{
    enum class button{
        CROSS,
        CIRCLE,
        TRIANGLE,
        SQUARE,
        L1,
        R1,
        L2,
        R2,
        CREATE,
        OPTION,
        HOME,
        L3,
        R3,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    enum class axis{
        LEFT_X,
        LEFT_Y,
        L3_DEPTH,
        RIGHT_X,
        RIGHT_Y,
        R3_DEPTH
    };

    extern const int left_y_num;
    extern const int right_y_num;
    extern const int cross_x_axis;
    extern const int cross_y_axis;
    extern const int left_num;
    extern const int right_num;
    extern const int up_num;
    extern const int down_num;


    extern const std::string wired_device_name;
    extern const std::string wireless_device_name;
    extern const int total_button;
    extern const int total_axis;

    class DualSense: public pad::GamePad{
    private:
        void button_process() override;
        void axis_process() override;

    public:
        DualSense();
        DualSense(std::string device_name);
        ~DualSense() override;

        bool is_ON(ds5::button button);
        bool is_Pushed(ds5::button button);
        bool is_Released(ds5::button button);
        int get_axis_value(ds5::axis axis);
        std::vector<bool> get_raw_button_data();
        std::vector<int16_t> get_raw_axis_data();
    };
}

#endif