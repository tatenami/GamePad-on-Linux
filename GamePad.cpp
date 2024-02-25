#include "GamePad.hpp"

const std::string device_info_file = "/proc/bus/input/devices";
static constexpr uint8_t event_type_filter = 0x7f;
static constexpr int default_total_button = 17;
static constexpr int default_total_axis = 6;

/*
* --- [ GamePad Class ] ---
*/

bool find_device_file(std::string device_name, Device_info& pad_info){
    bool judge = false;
    int pos;
    std::string read_buf;
    std::ifstream read_stream;

    read_stream.open(device_info_file, std::ios::in);

    while(std::getline(read_stream, read_buf)){
        if(read_buf.find(device_name) != std::string::npos){
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

    pad_info.device_file = "/dev/input/" + read_buf.substr(pos, 3);
    return judge;
}

GamePad::GamePad(std::string device_name){
    pad_info.connection = find_device_file(device_name, this->pad_info);
    pad_info.device_name = device_name;

    if(!(pad_info.connection)){
        std::cout << "Fail to connect [ " << device_name << " ]" << std::endl;
    }

    pad_info.device_fd = open(pad_info.device_file.c_str(), O_RDONLY);

    if(read(pad_info.device_fd, &(pad_info.event), sizeof(pad_info.event)) == -1){
        std::cout << "Fail to read data" << std::endl;
    }

    button_data = std::vector<bool>(default_total_button);
    pre_button_data = std::vector<bool>(default_total_axis);
    axis_data = std::vector<int16_t>(default_total_axis);
}

GamePad::GamePad(std::string device_name, int total_button, int total_axis):
    GamePad(device_name)
{   
    button_data.resize(total_button);
    pre_button_data.resize(total_button);
    axis_data.resize(total_axis);
}

GamePad::~GamePad(){
    close(pad_info.device_fd);
}

bool GamePad::is_connect(){
    return pad_info.connection;
}

void GamePad::data_process(){
    switch(pad_info.event.type & event_type_filter){
        case JS_EVENT_BUTTON:
            button_process();
            break;
        case JS_EVENT_AXIS:
            axis_process();
            break;
    }
}

bool GamePad::load_data(){
    int size;
    size = read(pad_info.device_fd, &(pad_info.event), sizeof(pad_info.event));
    if(size == sizeof(pad_info.event)){
        pad_info.connection = true;
        data_process();
    }
    else{
        pad_info.connection = false;
        std::cout << "fail to read pad data" << std::endl;
    }
    return pad_info.connection;
}

bool GamePad::is_ON(int number){
    return button_data[number];
}

bool GamePad::is_ON(Button_type type){
    bool state = false;
    for(auto e: button_data){
        if(e){
            state = true;
            break;
        }
    }

    if(type == Button_type::None){
        return ~state;
    }
    return state;
}

bool GamePad::is_Pushed(int number){
    return (pre_button_data[number] ^ button_data[number]) & button_data[number];
}

bool GamePad::is_Released(int number){
    return (pre_button_data[number] ^ button_data[number]) & ~button_data[number];
}

int GamePad::get_axis_value(int number){
    return (int)axis_data[number];
}