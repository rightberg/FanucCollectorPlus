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
#include "focas/fwlib32.h"
#include "Fanuc.h"
#include "Collector.h"
#define _CRT_SECURE_NO_WARNINGS

std::atomic_bool running{ true };

static BOOL WINAPI ConsoleHandlerRoutine(DWORD input_type)
{
    switch (input_type)
    {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            running = false;
            return TRUE;
        default:
            return FALSE;
    }
}
std::vector<Device> devices = {};

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <json_string>" << std::endl;
        return 1;
    }

    try
    {
        std::string json_string = argv[1];
        bool error = ParseDevices(json_string.c_str(), devices);
        if (!error)
        {
            std::cerr << "Ошибка парсинга devices" << std::endl;
            return 2;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 3;
    }

    if (!SetConsoleCtrlHandler(ConsoleHandlerRoutine, TRUE))
    {
        std::cerr << "Не удалось установить обработчик консольных событий." << std::endl;
        return 4;
    }

    while(running && !std::cin.eof())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        try
        {
            for (const auto& device : devices)
            {
                ushort_data handle = GetHandle(device.address, device.port, 1);
                FanucData collector = {};
                SetFanucData(handle.data, device, collector);
                if (!handle.IsError())
                    FreeHandle(handle.data);
                std::string serialized_data = SerializeFanucData(collector);
                std::cout << serialized_data << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return 5;
        }
    }
    return 0;
}