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
    int delay_ms;
};

struct FanucData
{
    //device data
    std::string name;
    std::string address;
    std::string series;
    int port;
    //mode data
    short aut;
    short run;
    short edit;
    short shutdowns;
    short hight_speed;
    short motion;
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
    std::map<std::string, long> absolute_positions;
    std::map<std::string, long> machine_positions;
    std::map<std::string, long> relative_positions;
    //spindle data
    short spindle_override;
    long spindle_speed;
    int spindle_param_speed;
    std::map<std::string, int> spindle_motor_speed;
    std::map<std::string, int> spindle_load;
    //alarm data
    short emergency;
    short alarm;
    //other data
    long power_on_time;
    long operation_time;
    long cutting_time;
    long cycle_time;
    std::string series_number;
    std::string version_number;
    //errors data
    std::vector<short> errors = std::vector<short>(37);
};

//process data
bool SetFanucData(unsigned short handle, const Device& device, FanucData& data);
bool ParseDevices(const char* json, std::vector<Device>& devices);
std::string SerializeFanucData(FanucData& data);
void FreeAllHandles();
#endif