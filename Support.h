#pragma once
#ifndef SUPPORT_H
#define SUPPORT_H
#include <string>
#include <atomic>

extern std::atomic_bool running;
extern std::atomic_bool exit_flag;
// debug functions
bool CreateCrashLog(const std::string& message);
// exit functions
bool SetSignalHandler();
bool TryGetPID(char* pid);
#endif