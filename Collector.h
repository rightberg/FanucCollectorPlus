#pragma once
#ifndef COLLECTOR_H
#define COLLECTOR_H
#include "FanucTypes.h"
#include <string>
#include <vector>

struct Device
{
    std::string name;
    std::string address;
    std::string series;
    int port;
};

struct FanucData
{
    //device data
    std::string name;
    std::string address;
    std::string series;
    int port;
    //mode data
    short mode;
    short run_state;
    short status;
    short shutdowns;
    short hight_speed;
    short axis_motion;
    short mstb;
    long load_excess;
    //program data
    std::string frame;
    short main_prog_number;
    short sub_prog_number;
    int parts_count;
    long tool_number;
    long frame_number;
    //axes data
    short jog_override;
    short feed_override;
    long feedrate;
    long jog_speed;
    float current_load;
    float current_load_percent;
    std::map<std::string, int> servo_loads;
    //spindle data
    short spindle_override;
    long spindle_speed;
    long spindle_param_speed;
    std::map<std::string, int> spindle_motor_speed;
    std::map<std::string, int> spindle_load;
    //alarm data
    short emergency;
    short alarm_status;
    //other data
    long power_on_time;
    long operating_time;
    long cutting_time;
    long cycle_time;
    //errors data
    std::vector<short> errors = std::vector<short>(32);
    std::vector<std::string> errors_str = std::vector<std::string>(32);
};

//process data
void SetFanucData(unsigned short handle, const Device& device, FanucData& data);
bool ParseDevices(const char* json, std::vector<Device>& devices);
std::string SerializeFanucData(FanucData& data);
#endif