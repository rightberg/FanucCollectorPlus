#pragma once
#ifndef FANUC_H
#define FANUC_H
#include <string>
#include "FanucTypes.h"

//decode functions
std::string GetCncErrorMessage(int error_code);
std::string GetModeString(short mode, std::string series);
std::string GetRunStateString(int state_num, std::string series);
std::string GetStatusString(int status);
std::string GetAxisMotionString(int motion, const std::string series);
std::string GetMstbString(int mstb, const std::string series);
std::string GetLoadExcessString(int mstb, const std::string series);
std::string GetHightSpeedString(int index);
std::string GetAlarmString(int alarm, const std::string series);
std::string GetAlarmStatusString(int alarm, const std::string series);
std::string GetEmergencyString(int emg, const std::string series);
std::string GetUnitString(int unit);
std::string GetShutdownsString(short index);
int GetSpindleOverrideValue(short sp_ov);
float GetJogOverrideValue(short jog);
int GetFeedOverrideValue(short unit);
//handle functions
ushort_data GetHandle(std::string address, int port, int timeout);
void_func FreeHandle(unsigned short handle);
//mode functions
short_data GetMode(unsigned short handle);
short_data GetRunState(unsigned short handle);
short_data GetStatus(unsigned short handle);
short_data GetShutdowns(unsigned short handle);
short_data GetHightSpeed(unsigned short handle);
short_data GetAxisMotion(unsigned short handle);
short_data GetMstb(unsigned short handle);
long_data GetLoadExcess(unsigned short handle);
//program data
short_data GetMainPrgNumber(unsigned short handle);
short_data GetSubPrgNumber(unsigned short handle);
str_data GetFrame(unsigned short handle);
int_data GetPartsCount(unsigned short handle);
long_data GetToolNumber(unsigned short handle);
long_data GetFrameNumber(unsigned short handle);
//axis data
long_data GetFeedRate(unsigned short handle);
short_data GetFeedOverride(unsigned short handle);
short_data GetJogOverride(unsigned short handle);
long_data GetJogSpeed(unsigned short handle);
float_data GetServoCurrentLoad(unsigned short handle);
float_data GetServoCurrentPercentLoad(unsigned short handle);
map_data GetAllServoLoad(unsigned short handle);
//spindle data
long_data GetSpindleSpeed(unsigned short handle);
long_data GetSpindleSpeedParam(unsigned short handle);
map_data GetSpindleMotorSpeed(unsigned short handle);
map_data GetSpindleLoad(unsigned short handle);
short_data GetSpindleOverride(unsigned short handle);
//alarm data
short_data GetEmergencyStop(unsigned short handle);
short_data GetAlarmStatus(unsigned short handle);
#endif