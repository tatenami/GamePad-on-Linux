#include "joy.h"

/* 
----------------[ Game Pad Class ]-----------------
*/

bool GamePad::find_device_file(std::string controller_name){
    bool judge = false;
    int pos;
    std::string read_buf;
    std::ifstream read_stream;
    std::string device_list_file = DEVICE_LIST_FILE_NAME;

    read_stream.open(device_list_file, std::ios::in);

    while(std::getline(read_stream, read_buf)){
        if(read_buf.find(controller_name) != std::string::npos){
            judge = true;
            break;
        }
    }

    if(!judge){
        return judge;
    }
    else{
        judge = false;
    }

    while(std::getline(read_stream, read_buf)){
        pos = read_buf.find("js");
        if(pos != std::string::npos){
            judge = true;
            break;
        }
    }

    if(!judge){
        return judge;
    }

    device_file = "/dev/input/" + read_buf.substr(pos, 3);

    return judge;
}

GamePad::GamePad(std::string controller_name){
    connection = find_device_file(controller_name);

    if(!connection){
        std::cout << "Fail to connect [ " << controller_name << " ]" << std::endl;
    }

    dev_fd = open(device_file.c_str(), O_RDONLY);
    
    if(read(dev_fd, &event, sizeof(event)) == -1){
        std::cout << "Fail to read data" << std::endl;
    }
}

bool GamePad::is_connect(){
    return connection;
}

void GamePad::set_Ystick_num(int left_num, int right_num){
    leftY_num = left_num;
    rightY_num = right_num;
}

void GamePad::data_process(){
    int axis_value;
    switch(event.type & TYPE_FILTER){
        case JS_EVENT_BUTTON:
            pre_button_data = button_data;
            button_data[event.number] = event.value;
            break;
        case JS_EVENT_AXIS:
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
            break;
    }
}

void GamePad::load_data(){
    if(read(dev_fd, &event, sizeof(event)) == sizeof(struct js_event)){
        connection = true;
        data_process();
    }
    else{
        connection = false;
        std::cout << "fail to read pad data" << std::endl;
    }
}

bool GamePad::is_ON(int button_number){
    bool is_on;
    if(button_number == ANY){
        if(button_data.any()){
            is_on = true;
        }
        else{
            is_on = false;
        }
    }
    else{
        is_on = button_data[button_number];
    }

    return is_on;
}

bool GamePad::is_OFF(int button_number){
    return ~button_data[button_number];
}

bool GamePad::is_Pushed(int button_number){
    return (pre_button_data[button_number] ^ button_data[button_number]) & is_ON(button_number);
}

bool GamePad::is_Rleased(int button_number){
    return (pre_button_data[button_number] ^ button_data[button_number]) & is_OFF(button_number);
}

int GamePad::get_stick_value(int stick_number){
    return (int)axis_data[stick_number];
}