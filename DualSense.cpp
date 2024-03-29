#include "DualSense.hpp"

const std::string ds5::wired_device_name = "\"Sony Interactive Entertainment DualSense Wireless Controller\"";
const std::string ds5::wireless_device_name = "\"DualSense Wireless Controller\"";

const int ds5::left_y_num = 1;
const int ds5::right_y_num = 4;
const int ds5::cross_x_axis = 6;
const int ds5::cross_y_axis = 7;
const int ds5::left_num = 13;
const int ds5::right_num = 14;
const int ds5::up_num = 15;
const int ds5::down_num = 16;

const int ds5::total_button = 17;
const int ds5::total_axis = 6;

const static int modified_limit = 2000;

ds5::DualSense::DualSense(): GamePad(ds5::wireless_device_name, ds5::total_button, ds5::total_axis)
{
    std::cout << "using DualSense [ Wireless ] state" << std::endl;
}

ds5::DualSense::DualSense(std::string device_name): GamePad(device_name, ds5::total_button, ds5::total_axis){
    if(device_name == ds5::wired_device_name){
        std::cout << "using DualSense [ Wired ] state" << std::endl;
    }
    else if(device_name == ds5::wireless_device_name){
        std::cout << "using DualSense [ Wireless ] state" << std::endl;
    }
}

ds5::DualSense::~DualSense(){
    std::cout << "Connection lost" << std::endl;
}

void ds5::DualSense::button_process(){
    basic_button_process();
}

void ds5::DualSense::axis_process(){
    int axis_value = pad_info.event.value;
    int event_num = pad_info.event.number;
    if(pad_info.event.number == ds5::cross_x_axis){
        if(axis_value > 0){
            button_data[ds5::right_num] = true;
            button_data[ds5::left_num] = false;
        }
        else if(axis_value < 0){
            button_data[ds5::left_num] = true;
            button_data[ds5::right_num] = false;
        }
        else{
            button_data[ds5::left_num] = false;
            button_data[ds5::right_num] = false;
        }
    }
    else if(event_num == ds5::cross_y_axis){
            if(axis_value > 0){
                button_data[ds5::down_num] = true;
                button_data[ds5::up_num] = false;
            }
            else if(axis_value < 0){
                button_data[ds5::up_num] = true;
                button_data[ds5::down_num] = false;
            }
            else{
                button_data[ds5::up_num] = false;
                button_data[ds5::down_num] = false;
            }
    }
    else if(event_num == ds5::left_y_num || event_num == ds5::right_y_num){
        axis_value *= -1;
    }

    if(abs(axis_value) < modified_limit){
        axis_value = 0;
    }
    axis_data[event_num] = static_cast<int16_t>(axis_value);
}

bool ds5::DualSense::is_ON(ds5::button button){
    return button_data[static_cast<int>(button)];
}

bool ds5::DualSense::is_Pushed(ds5::button button){
    return GamePad::is_Pushed(static_cast<int>(button));
}

bool ds5::DualSense::is_Released(ds5::button button){
    return GamePad::is_Released(static_cast<int>(button));
}

int ds5::DualSense::get_axis_value(ds5::axis axis){
    return GamePad::get_axis_value(static_cast<int>(axis));
}

std::vector<bool> ds5::DualSense::get_raw_button_data(){
    return button_data;
}

std::vector<int16_t> ds5::DualSense::get_raw_axis_data(){
    return axis_data;
}