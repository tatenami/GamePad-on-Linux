#ifndef DUAL_SENSE_H
#define DUAL_SENSE_H

#include "GamePad.hpp"

namespace ds{
    enum class button{
        cross,
        circle,
        triangle,
        square,
        L1,
        R1,
        L2,
        R2,
        create,
        option,
        home,
        L3,
        R3,
        left,
        right,
        up,
        down
    };
    enum class axis{
        left_x,
        left_y,
        L3_depth,
        right_x,
        right_y,
        R3_depth
    };

    extern const int left_y_num;
    extern const int right_y_num;
    extern const int cross_x_axis;
    extern const int cross_y_axis;

    extern const std::string wired_device_name;
    extern const std::string wireless_device_name;
    extern const int total_button;
    extern const int total_axis;
}

class DualSense: public GamePad{
private:
    void button_process() override;
    void axis_process() override;

public:
    DualSense();
    DualSense(std::string device_name);
    ~DualSense() override;

    using GamePad::is_ON;
    using GamePad::is_Pushed;
    using GamePad::is_Released;
    using GamePad::get_axis_value;
    bool is_ON(ds::button button);
    bool is_Pushed(ds::button button);
    bool is_Released(ds::button button);
    int get_axis_value(ds::axis axis);
};

#endif