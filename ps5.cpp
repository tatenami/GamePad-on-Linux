#include "ps5.h"

void DualSense::button_process(){
    pre_button_data = button_data;
    button_data[event.number] = event.value;
}

void DualSense::axis_process(){
    int axis_value;
    if(event.number == 6){
        if(event.value > 0){
                button_data[14] = ON;
                button_data[13] = OFF;
            }
            else if(event.value < 0){
                button_data[13] = ON;
                button_data[14] = OFF;
            }
            else{
                button_data[13] = OFF;
                button_data[14] = OFF;
            }
        }   
        else if(event.number == 7){
            if(event.value > 0){
                button_data[16] = ON;
                button_data[15] = OFF;
            }
            else if(event.value < 0){
                button_data[15] = ON;
                button_data[16] = OFF;
            }
            else{
                button_data[15] = OFF;
                button_data[16] = OFF;
            }
        }
        else{
            axis_value = event.value * RATE_8B_per_16B;
            if(event.number == leftY_num || event.number == rightY_num){
                axis_value *= -1;
            }
        }

    if(abs(axis_value) < 5){
        axis_value = 0;
    }
    axis_data[event.number] = (int8_t)axis_value;
}

void DualSense::data_process(){
    switch(event.type & TYPE_FILTER){
        case JS_EVENT_BUTTON:
            button_process();
            break;
        case JS_EVENT_AXIS:
            axis_process();
            break;
    }
}

DualSense::DualSense(std::string controller_name): GamePad(controller_name, TOTAL_BUTTON, TOTAL_AXIS)
{
    set_Ystick_num(LEFT_Y, RIGHT_Y);
}

void DualSense::get_stick_arg(double& left_arg, double& right_arg){
    left_arg = atan2(axis_data[LEFT_Y], axis_data[LEFT_X]);
    right_arg = atan2(axis_data[RIGHT_Y], axis_data[RIGHT_X]);
}