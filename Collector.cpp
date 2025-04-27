#pragma warning(disable: 4996)
#pragma comment(lib, "focas/fwlib32.lib")
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include "focas/fwlib32.h"
#include "Fanuc.h"
#include "Collector.h"
#include "Support.h"
#define _CRT_SECURE_NO_WARNINGS

std::mutex cout_mutex;
std::vector<Device> devices = {};
std::vector<unsigned short> handles = {};
int max_errors = 10;
int min_delay_ms = 50;
int  handle_timeout = 10;

void FreeAllHandles()
{
    int max_handles = handles.size();
    for (int i = 0; i < max_handles; i++)
        FreeHandle(handles[i]);
}

void CollectDataThread(Device device, unsigned short& handle, std::atomic_bool& running_flag)
{
    int delay_ms = device.delay_ms;
    if (delay_ms < min_delay_ms)
        delay_ms = min_delay_ms;
    handle = GetHandle(device.address, device.port, handle_timeout).data;
    int errors_counter = 0;
    while (running_flag)
    {
        FanucData collector = {};
        if (!SetFanucData(handle, device, collector))
        {
            errors_counter++;
            if (errors_counter == max_errors)
            {
                errors_counter = 0;
                FreeHandle(handle);
                handle = GetHandle(device.address, device.port, handle_timeout).data;
            }
        }
        std::string serialized_data = SerializeFanucData(collector);
        {  
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << serialized_data << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    FreeHandle(handle);
}

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argc < 2)
    {
        std::string message = std::string("Ожидается <json_string>\n");
        std::cerr << message;
        CreateCrashLog(message);
        return 1;
    }

    try
    {
        std::string json_string = argv[1];
        if (!ParseDevices(json_string.c_str(), devices))
        {
            std::string message = std::string("Ошибка парсинга devices\n");
            std::cerr << message;
            CreateCrashLog(message);
            return 2;
        }
    }
    catch (const std::exception& e)
    {
        std::string message = std::string("Ошибка парсинга (аргумент)\n");
        std::cerr << message;
        CreateCrashLog(message);
        return 3;
    }

    if (argc >= 3) 
    {
        if (!TryGetPID(argv[2]))
        {
            std::string message = std::string("Некорректный PID: ") + argv[2] + "\n";
            std::cerr << message;
            CreateCrashLog(message);
            return 3;
        }
    }

    if (argc >= 4)
    {
        std::string arg = argv[3];
        int number = std::stoi(arg);
        if (number >= 1)
            handle_timeout = number;
    }

    if (!SetSignalHandler())
    {
        std::string message = "Не удалось установить обработчик консольных событий\n";
        std::cerr << message;
        CreateCrashLog(message);
        return 4;
    }

    std::vector<std::thread> threads;
    int device_count = devices.size();
    handles = std::vector<unsigned short>(device_count);
    for (int i = 0; i < device_count; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        threads.emplace_back(CollectDataThread, devices[i], std::ref(handles[i]), std::ref(running));
    }

    for (auto& thread : threads) 
        thread.join();

    exit_flag = true;
    return 0;
}