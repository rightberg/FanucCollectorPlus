#pragma once
#ifndef COLLECTOR_H
#define COLLECTOR_H
#include <string>
#include <vector>
#include <atomic>

extern std::atomic_bool running;
extern std::atomic_bool exit_flag;

struct Device
{
    std::string name;
    std::string address;
    std::vector<std::string> pack;
    int port = 8193;
    int delay_ms = 50;
};

//process data
bool ParseDevices(const char* json, std::vector<Device>& devices);
void FreeAllHandles();
void GetFanucDataJson(unsigned short& handle, const Device& device, std::string& json_data);
void InitTagPacks(std::vector<Device>& devices);
//file functions
bool CreateCrashLog(const std::string& message);
//exit functions
bool SetSignalHandler();
bool TryGetPID(char* pid);
#endif