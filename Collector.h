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
    short g00;
    short motion;
    short mstb;
    short load_excess;
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
    std::map<std::string, int> servo_loads;
    std::map<std::string, double> absolute_positions;
    std::map<std::string, double> machine_positions;
    std::map<std::string, double> relative_positions;
    std::map<std::string, long> current_load_percent;
    std::map<std::string, double> current_load;
    std::map<std::string, double> jog_speed;
    //spindle data
    short spindle_override;
    long spindle_speed;
    std::map<std::string, int> spindle_motor_speed;
    std::map<std::string, int> spindle_load;
    std::map<std::string, long> spindle_param_speed;
    //alarm data
    short emergency;
    short alarm;
    //other data
    short axes_number;
    short spindles_number;
    short channels_number;
    long power_on_time;
    double operation_time;
    double cutting_time;
    double cycle_time;
    std::string series_number;
    std::string version_number;
    //errors data
    std::vector<short> errors = std::vector<short>(40);
};

//process data
bool SetFanucData(unsigned short handle, const Device& device, FanucData& data);
bool ParseDevices(const char* json, std::vector<Device>& devices);
std::string SerializeFanucData(FanucData& data);
void FreeAllHandles();
#endif