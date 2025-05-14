#include <string>
#include <cstdlib>
#include <windows.h>
#include <iostream>
#include <thread>
#include "Collector.h"

std::atomic_bool running{ true };
std::atomic_bool exit_flag{ false };

static BOOL WINAPI ConsoleHandlerRoutine(DWORD input_type)
{
    int time_couter = 0;
    int delay_ms = 50;
    int max_counter = 3000;
    switch (input_type)
    {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            running = false;
            while (!exit_flag)
            {
                Sleep(delay_ms);
                time_couter += delay_ms;
                if (time_couter >= max_counter)
                {
                    CreateCrashLog("Экстренное освобождение handles");
                    FreeAllHandles();
                    break;
                }
            }
            return TRUE;
        default:
            return FALSE;
    }
}

bool SetSignalHandler()
{
    if (!SetConsoleCtrlHandler(ConsoleHandlerRoutine, TRUE))
        return false;
    else
        return true;
}

static void WaitForParentShutdown(DWORD parent_pid)
{
    HANDLE parent_handle = OpenProcess(SYNCHRONIZE, FALSE, parent_pid);
    if (parent_handle == NULL)
    {
        std::cerr << "Не удалось открыть процесс родителя. Ошибка: " << GetLastError() << std::endl;
        running = false;
        return;
    }

    DWORD wait_result = WaitForSingleObject(parent_handle, INFINITE);
    if (wait_result == WAIT_OBJECT_0)
    {
        std::cerr << "Обнаружено завершение родительского процесса. Завершаем collector.exe." << std::endl;
        running = false;
    }
    else
        std::cerr << "Ошибка ожидания завершения родителя, код: " << wait_result << std::endl;
    CloseHandle(parent_handle);
}


bool TryGetPID(char* pid)
{
    try
    {
        DWORD parent_pid = static_cast<DWORD>(std::stoul(pid));
        std::thread watch_parent(WaitForParentShutdown, parent_pid);
        watch_parent.detach();
        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}
