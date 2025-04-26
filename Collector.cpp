#pragma warning(disable: 4996)
#pragma comment(lib, "focas/fwlib32.lib")
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <thread>
#include "focas/fwlib32.h"
#include "Fanuc.h"
#include "Collector.h"
#include "Support.h"
#define _CRT_SECURE_NO_WARNINGS

std::vector<Device> devices = {};
std::vector<unsigned short> handles = {};
std::vector<int> errors_counter = {};
int max_errors = 10;

static void FreeReceivedHandles()
{
    int h_count = handles.size();
    for (int i = 0; i < h_count; i++)
        FreeHandle(handles[i]);
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

    if (!SetSignalHandler())
    {
        std::string message = "Не удалось установить обработчик консольных событий\n";
        std::cerr << message;
        CreateCrashLog(message);
        return 4;
    }

    int device_count = devices.size();
    try
    {
        handles = std::vector<unsigned short>(device_count);
        errors_counter = std::vector<int>(device_count);
        for (int i = 0; i < device_count; i++)
        {
            ushort_data handle = GetHandle(devices[i].address, devices[i].port, 5);
            handles[i] = handle.data;
        }
    }
    catch (const std::exception& e)
    {
        std::string message = std::string("Ошибка получения handles: ") + e.what();
        std::cerr << message;
        CreateCrashLog(message);
        return 5;
    }

    while(running && !std::cin.eof())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        try
        {
            for (int i = 0; i < device_count; i++)
            {
                FanucData collector = {};
                if (SetFanucData(handles[i], devices[i], collector))
                {
                    std::string serialized_data = SerializeFanucData(collector);
                    std::cout << serialized_data << std::endl;
                }
                else
                {
                    errors_counter[i]++;
                    if (errors_counter[i] == max_errors)
                    {
                        errors_counter[i] = 0;
                        FreeHandle(handles[i]);
                        ushort_data handle = GetHandle(devices[i].address, devices[i].port, 5);
                        handles[i] = handle.data;
                    }
                }
            }
        }
        catch (const std::exception& e)
        {
            std::string message = std::string("Ошибка работы с данными: ") + e.what();
            std::cerr << message;
            CreateCrashLog(message);
            return 6;
        }
    }
    FreeReceivedHandles();
    return 0;
}