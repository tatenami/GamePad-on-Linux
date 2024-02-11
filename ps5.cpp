#include "ps5.h"

void DualSense::data_process(){
    int axis_value;
    switch(event.type & TYPE_FILTER){
        case JS_EVENT_BUTTON:
            pre_button_data = button_data;
            button_data[event.number] = event.value;
            break;
        case JS_EVENT_AXIS:
            if(event.number == CROSS_X_AXIS){
                if(event.value > 0){
                    button_data[RIGHT] = ON;
                    button_data[LEFT] = OFF;
                }
                else if(event.value < 0){
                    button_data[LEFT] = ON;
                    button_data[RIGHT] = OFF;
                }
                else{
                    button_data[LEFT] = OFF;
                    button_data[RIGHT] = OFF;
                }
            }   
            else if(event.number == CROSS_Y_AXIS){
                if(event.value > 0){
                    button_data[DOWN] = ON;
                    button_data[UP] = OFF;
                }
                else if(event.value < 0){
                    button_data[UP] = ON;
                    button_data[DOWN] = OFF;
                }
                else{
                    button_data[UP] = OFF;
                    button_data[DOWN] = OFF;
                }
            }
            else{
                axis_value = event.value * RATE_8B_per_16B;
                if(event.number == leftY_num || event.number == rightY_num){
                    axis_value *= -1;
                }
            }

            if(abs(axis_value) <= 5){
                axis_value = 0;
            }
            axis_data[event.number] = (int8_t)axis_value;
            break;
    }
}

DualSense::DualSense(std::string controller_name): GamePad(controller_name)
{
    set_Ystick_num(LEFT_Y, RIGHT_Y);
}

void DualSense::get_stick_arg(double& left_arg, double& right_arg){
    left_arg = atan2(axis_data[LEFT_Y], axis_data[LEFT_X]);
    right_arg = atan2(axis_data[RIGHT_Y], axis_data[RIGHT_X]);
}