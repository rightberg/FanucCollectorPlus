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
#include <chrono>
#include "focas/fwlib32.h"
#include "Fanuc.h"
#include "Collector.h"
#define _CRT_SECURE_NO_WARNINGS

std::mutex cout_mutex;
std::vector<Device> devices = {};
std::vector<unsigned short> handles = {};
int max_errors = 1;
int  handle_timeout = 10;
int free_time = 10;

void FreeAllHandles()
{
    int max_handles = handles.size();
    for (int i = 0; i < max_handles; i++)
        FreeHandle(handles[i]);
}

static void CollectDataThread(Device device, unsigned short& handle, std::atomic_bool& running_flag)
{
    int delay_ms = device.delay_ms;
    if (delay_ms < 0)
        delay_ms = 0;
    UShortData _handle = {};
    VoidFunc free_handle = {};
    unsigned short stacked_handle = 0;
    short power_on = 1;
    std::string json_data;
    while (running_flag)
    {
        if (stacked_handle != 0)
        {
            free_handle = FreeHandle(stacked_handle);
            if (free_handle.error == 0 || free_handle.error == EW_HANDLE)
            {
                stacked_handle = 0;
                std::cerr << "Освобождение дескриптора: успешно" << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(free_time));
        }
        else
        {
            _handle = GetHandle(device.address, device.port, handle_timeout);
            if (_handle.error == 0)
            {
                power_on = 1;
                handle = _handle.data;
                GetFanucDataJson(handle, _handle.error, device, json_data);
                {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << json_data << std::endl;
                }
                free_handle = FreeHandle(handle);
                if (free_handle.error != 0 && stacked_handle == 0 && handle != 0)
                {
                    stacked_handle = handle;
                    std::cerr 
                        << "Ошибка освобождения дескриптора, handle: " << handle 
                        << ", error: " << free_handle.error  
                        << std::endl;
                }
            }
            else if (_handle.error == EW_SOCKET)
            {
                if (power_on == 1)
                {
                    std::cerr << "Отсутсвует питание устройства (EW_SOCKET: -16)" << std::endl;
                    GetFanucDataJson(handle, _handle.error, device, json_data);
                    {
                        std::lock_guard<std::mutex> lock(cout_mutex);
                        std::cout << json_data << std::endl;
                    }
                    power_on = 0;
                }
            }
            else
                std::cerr << "Ошибка получения дескриптора, error: " << _handle.error << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
}

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argc < 2)
    {
        std::cerr << "Ожидается <json_string>" << std::endl;
        return 1;
    }

    if (argc >= 2)
        if (!ParseDevices(argv[1], devices))
        {
            std::cerr << "Ошибка парсинга devices" << std::endl;
            return 2;
        }

    if (argc >= 3) 
        if (!TryGetPID(argv[2]))
        {
            std::cerr << "Некорректный PID: " << std::endl;
            return 3;
        }

    if (argc >= 4)
    {
        int number = std::stoi(argv[3]);
        if (number >= 1)
            handle_timeout = number;
    }

    if (!SetSignalHandler())
    {
        std::cerr << "Не удалось установить обработчик консольных событий: " << std::endl;
        return 4;
    }

    InitTagPacks(devices);

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