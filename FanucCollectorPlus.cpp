#pragma warning(disable: 4996)
#pragma comment(lib, "focas/fwlib32.lib")
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "focas/fwlib32.h"
#include "Fanuc.h"
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#define _CRT_SECURE_NO_WARNINGS

struct Device
{
    std::string address;
    std::string series;
    int port;

    friend void to_json(nlohmann::json& j, const Device& d)
    {
        j = nlohmann::json{ {"address", d.address}, {"port", d.port}, {"series", d.series} };
    }

    static friend void from_json(const nlohmann::json& j, Device& d)
    {
        j.at("address").get_to(d.address);
        j.at("port").get_to(d.port);
        j.at("series").get_to(d.series);
    }
};

struct ModeData
{
    std::string mode;
    std::string run_state;
    std::string status;
    std::string shutdowns;
    std::string hight_speed;
    std::string axis_motion;
    std::string mstb;
    std::string load_excess;

    ModeData(unsigned short handle, const std::string& series)
    {
        mode = GetMode(handle, series).data;
        run_state = GetRunState(handle, series).data;
        status = GetStatus(handle, series).data;
        shutdowns = GetShutdowns(handle).data;
        hight_speed = GetHightSpeed(handle).data;
        axis_motion = GetAxisMotion(handle, series).data;
        mstb = GetMstb(handle, series).data;
        load_excess = GetLoadExcess(handle, series).data;
    }

    static friend void to_json(nlohmann::json& j, const ModeData& data)
    {
        j = nlohmann::json
        {
            {"mode", data.mode},
            {"run_state", data.run_state},
            {"status", data.status},
            {"shutdowns", data.shutdowns},
            {"hight_speed", data.hight_speed},
            {"axis_motion", data.axis_motion},
            {"mstb", data.mstb},
            {"load_excess", data.load_excess}
        };
    };
};

struct ProgramData
{
    std::string frame;
    int main_prog_number;
    int sub_prog_number;
    int parts_count;
    int tool_number;
    int frame_number;

    ProgramData(unsigned short handle)
    {
        frame = GetFrame(handle).data;
        main_prog_number = GetMainPrgNumber(handle).data;
        sub_prog_number = GetSubPrgNumber(handle).data;
        parts_count = GetPartsCount(handle).data;
        tool_number = GetToolNumber(handle).data;
        frame_number = GetFrameNum(handle).data;
    }

    static friend void to_json(nlohmann::json& j, const ProgramData& data)
    {
        j = nlohmann::json
        {
            {"frame", data.frame},
            {"main_prog_number", data.main_prog_number},
            {"sub_prog_number", data.sub_prog_number},
            {"parts_count", data.parts_count},
            {"tool_number", data.tool_number},
            {"frame_number", data.frame_number}
        };
    };
};

struct AxesData
{
    int feedrate;
    int feed_override;
    float jog_override;
    int jog_speed;
    float current_load;
    float current_load_percent;
    std::map<std::string, int> servo_loads;

    AxesData(unsigned short handle) {
        feedrate = GetFeedRate(handle).data;
        feed_override = GetFeedOverride(handle).data;
        jog_override = GetJogOverride(handle).data;
        jog_speed = GetJogSpeed(handle).data;
        current_load = GetServoCurrentLoad(handle).data;
        current_load_percent = GetServoCurrentPercentLoad(handle).data;
        servo_loads = GetAllServoLoad(handle).data;
    }

    static friend void to_json(nlohmann::json& j, const AxesData& data)
    {
        j = nlohmann::json
        {
            {"feedrate", data.feedrate},
            {"feed_override", data.feed_override},
            {"jog_override", data.jog_override},
            {"jog_speed", data.jog_speed},
            {"current_load", data.current_load},
            {"current_load_percent", data.current_load_percent},
            {"servo_loads", data.servo_loads}
        };
    }
};

struct Collector
{
    Device device;
    ModeData mode_data;
    ProgramData program_data;
    AxesData axes_data;

    Collector(unsigned short handle, const Device device) :
        device(device),
        mode_data(handle, device.series),
        program_data(handle),
        axes_data(handle)
    {
    }

    static friend void to_json(nlohmann::json& j, const Collector& collector)
    {
        j = nlohmann::json
        {
            {"device", collector.device},
            {"mode_data", collector.mode_data},
            {"program_data", collector.program_data},
            {"axes_data", collector.axes_data}
        };
    };
};

class CollectorData
{
public: std::vector<Collector> collectors;

      void AddCollector(unsigned short handle, Device device)
      {
          collectors.emplace_back(handle, device);
      }

      static friend void to_json(nlohmann::json& j, const CollectorData& collector_data)
      {
          j = nlohmann::json{ {"collectors", collector_data.collectors} };
      }
};

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
        auto parsed_json = nlohmann::json::parse(json_string);
        auto devices_array = parsed_json.get<std::vector<Device>>();

        CollectorData col_data;
        for (const auto& device : devices_array)
        {
            ushort_data handle = GetHandle(device.address, device.port, 10);
            if (handle.error)
            {
                col_data.AddCollector(handle.data, device);
                void_func free_handle = FreeHandle(handle.data);
            }
        }

        nlohmann::json output_json = col_data;
        std::cout << output_json.dump() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}