#pragma once
#ifndef FANUC_H
#define FANUC_H
#include <string>
#include "FanucTypes.h"
//handle functions
UShortData GetHandle(std::string& address, int& port, int& timeout);
VoidFunc FreeHandle(unsigned short& handle);
//mode functions
ShortData GetAut(unsigned short& handle);
ShortData GetRun(unsigned short& handle);
ShortData GetEdit(unsigned short& handle);
ShortData GetShutdowns(unsigned short& handle);
ShortData GetG00(unsigned short& handle);
ShortData GetMotion(unsigned short& handle);
ShortData GetMstb(unsigned short& handle);
ShortData GetLoadExcess(unsigned short& handle);
//program data
ShortData GetMainPrgNumber(unsigned short& handle);
ShortData GetSubPrgNumber(unsigned short& handle);
StrData GetFrame(unsigned short& handle);
LongData GetPartsCount(unsigned short& handle);
LongData GetToolNumber(unsigned short& handle);
LongData GetFrameNumber(unsigned short& handle);
//axis data
LongData GetFeedRate(unsigned short& handle);
ShortData GetFeedOverride(unsigned short& handle);
ShortData GetJogOverride(unsigned short& handle);
DoubleMapData GetJogSpeed(unsigned short& handle);
DoubleMapData GetServoCurrentLoad(unsigned short& handle);
LongMapData GetServoCurrentPercentLoad(unsigned short& handle);
LongMapData GetAllServoLoad(unsigned short& handle);
DoubleMapData GetAbsolutePositions(unsigned short& handle);
DoubleMapData GetMachinePositions(unsigned short& handle);
DoubleMapData GetRelativePositions(unsigned short& handle);
//spindle data
DoubleData GetSpindleSpeed(unsigned short& handle);
LongMapData GetSpindleMotorSpeed(unsigned short& handle);
LongMapData GetSpindleLoad(unsigned short& handle);
ShortData GetSpindleOverride(unsigned short& handle);
LongMapData GetSpindleSpeedParam(unsigned short& handle);
//alarm data
ShortData GetEmergency(unsigned short& handle);
ShortData GetAlarm(unsigned short& handle);
//other data
ShortData GetCtrlAxesNumber(unsigned short& handle);
ShortData GetCtrlSpindlesNumber(unsigned short& handle);
ShortData GetCtrlPathsNumber(unsigned short& handle);
LongData GetPowerOnTime(unsigned short& handle);
DoubleData GetOperationTime(unsigned short& handle);
DoubleData GetCuttingTime(unsigned short& handle);
DoubleData GetCycleTime(unsigned short& handle);
StrData GetSeriesNumber(unsigned short& handle);
StrData GetVersionNumber(unsigned short& handle);
LongData GetSerialNumber(unsigned short& handle);
StrData GetCncId(unsigned short& handle);
#endif