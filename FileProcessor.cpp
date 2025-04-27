#include <windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

bool CreateCrashLog(const std::string& message)
{
    char exe_path[MAX_PATH];
    if (GetModuleFileNameA(NULL, exe_path, MAX_PATH) == 0)
        return false;
    try 
    {
        std::filesystem::path path_exe(exe_path);
        std::filesystem::path dir_path = path_exe.parent_path();
        std::filesystem::path log_path = dir_path / "crash_collector.log";
        std::ofstream logfile(log_path);
        if (!logfile.is_open()) 
        {
            std::cerr << "Ошибка при создании файла " << log_path.string() << std::endl;
            return false;
        }

        logfile << message;
        logfile.close();
        std::cout << "log файл успешно создан" << std::endl;
        return true;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Исключение при создании log: " << e.what() << std::endl;
        return false;
    }
}