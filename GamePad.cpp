#include "GamePad.hpp"

namespace pad{

    const std::string device_info_file = "/proc/bus/input/devices";
    static constexpr uint8_t event_type_filter = 0x7f;
    static constexpr int default_total_button = 17;
    static constexpr int default_total_axis = 6;

    /*
    * --- [ GamePad Class ] ---
    */

    bool GamePad::link_device_file(std::string device_name){
        bool connection_state;
        int pos;
        std::string msg, read_buf;
        std::ifstream read_stream(device_info_file);

        try{
            while(std::getline(read_stream, read_buf)){
                if(read_buf.find(device_name) != std::string::npos){
                    connection_state = true;
                    break;
                }
                else{
                    connection_state = false;
                }
            }
            if(connection_state == false){
                msg = "Fail to find target device";
                throw msg;
            }

            while(std::getline(read_stream, read_buf)){
                pos = read_buf.find("js");
                if(pos != std::string::npos){
                    connection_state = true;
                    break;
                }
                else{
                    connection_state = false;
                    msg = "Fail to find device file";
                    throw msg;
                }
            }
        
            read_stream.close();

            pad_info.device_file = "/dev/input/" + read_buf.substr(pos, 3);

            pad_info.device_fd = open(pad_info.device_file.c_str(), O_RDONLY);

            if(pad_info.device_fd == -1){
                connection_state = false;
                msg = "Fail to open device file";
                throw msg;
            }

            if(read(pad_info.device_fd, &(pad_info.event), sizeof(pad_info.event)) == -1){
                connection_state = false;
                msg = "Fail to read data";
                throw msg;
            }

            connection_state = true;

        }catch(std::string msg){
            std::cout << msg << std::endl;
            return false;
        }

        std::cout << "Initialized controller" << std::endl;
        return connection_state;
    }

    GamePad::GamePad(std::string device_name){
        pad_info.connection = link_device_file(device_name);
        pad_info.device_name = device_name;

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
        if(pad_info.connection){
            size = read(pad_info.device_fd, &(pad_info.event), sizeof(pad_info.event));                 
            if(size == sizeof(pad_info.event)){
                pad_info.connection = true;
                pre_button_data = button_data;
                data_process();
            }
            else{
                pad_info.connection = false;
            }
        }
        else{
            close(pad_info.device_fd);
            if(!link_device_file(pad_info.device_name)){
                pad_info.connection = false;
            }
            else{
                pad_info.connection = true;
                std::cout << "reconnect device" << std::endl;
            }
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
        return static_cast<int>(axis_data[number]);
    }

}