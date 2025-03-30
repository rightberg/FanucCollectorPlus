#pragma once
#ifndef FANUC_H
#define FANUC_H
#include <string>
#include "FanucTypes.h"

//sub functions
std::string GetCncErrorMessage(int error_code);
std::string GetModeString(short mode, std::string series);
//handle functions
ushort_data GetHandle(std::string address, int port, int timeout);
void_func FreeHandle(unsigned short handle);
//mode functions
str_data GetMainPrgName(unsigned short handle);
str_data GetMode(unsigned short handle, std::string series);
str_data GetRunState(unsigned short handle, std::string series);
str_data GetStatus(unsigned short handle, std::string series);
str_data GetShutdowns(unsigned short handle);
str_data GetHightSpeed(unsigned short handle);
str_data GetAxisMotion(unsigned short handle, std::string series);
str_data GetMstb(unsigned short handle, std::string series);
str_data GetLoadExcess(unsigned short handle, std::string series);
//program data
int_data GetMainPrgNumber(unsigned short handle);
int_data GetSubPrgNumber(unsigned short handle);
str_data GetFrame(unsigned short handle);
int_data GetPartsCount(unsigned short handle);
int_data GetToolNumber(unsigned short handle);
int_data GetFrameNum(unsigned short handle);
//axis data
int_data GetFeedRate(unsigned short handle);
int_data GetFeedOverride(unsigned short handle);
float_data GetJogOverride(unsigned short handle);
int_data GetJogSpeed(unsigned short handle);
float_data GetServoCurrentLoad(unsigned short handle);
float_data GetServoCurrentPercentLoad(unsigned short handle);
map_data GetAllServoLoad(unsigned short handle);
//spindle data
int_data GetSpindleSpeed(unsigned short handle);
int_data GetSpindleSpeedParam(unsigned short handle);
map_data GetSpindleMotorSpeed(unsigned short handle);
map_data GetSpindleLoad(unsigned short handle);
int_data GetSpindleOverride(unsigned short handle);
//alarm data
str_data GetEmergencyStop(unsigned short handle, std::string series);
str_data GetAlarmStatus(unsigned short handle, std::string series);
#endif