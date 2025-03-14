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
    std::string mode_err = "";

    ModeData(unsigned short handle, const std::string& series)
    {
        str_data _mode = GetMode(handle, series);
        str_data _run_state = GetRunState(handle, series);
        str_data _status = GetStatus(handle, series);
        str_data _shutdowns = GetShutdowns(handle);
        str_data _hight_speed = GetHightSpeed(handle);
        str_data _axis_motion = GetAxisMotion(handle, series);
        str_data _mstb = GetMstb(handle, series);
        str_data _load_excess = GetLoadExcess(handle, series);

        mode = _mode.data;
        run_state = _run_state.data;
        status = _status.data;
        shutdowns = _shutdowns.data;
        hight_speed = _hight_speed.data;
        axis_motion = _axis_motion.data;
        mstb = _mstb.data;
        load_excess = _load_excess.data;

        if (_mode.error)
            mode_err += " MODE ERR: " + _mode.error_msg;
        if (_run_state.error)
            mode_err += " RUN STATE ERR: " + _run_state.error_msg;
        if (_status.error)
            mode_err += " STATUS ERR: " + _status.error_msg;
        if (_shutdowns.error)
            mode_err += " SHUTDOWNS ERR: " + _shutdowns.error_msg;
        if (_hight_speed.error)
            mode_err += " HIGHT SPEED ERR: " + _hight_speed.error_msg;
        if (_axis_motion.error)
            mode_err += " AXIS MOTION ERR: " + _axis_motion.error_msg;
        if (_mstb.error)
            mode_err += " MSTB ERR: " + _mstb.error_msg;
        if (_load_excess.error)
            mode_err += " LOAD EXCESS ERR: " + _load_excess.error_msg;
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
            {"load_excess", data.load_excess},
            {"mode_err", data.mode_err}
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
    std::string prg_err = "";

    ProgramData(unsigned short handle)
    {
        str_data _frame = GetFrame(handle);
        int_data _main_prog_number = GetMainPrgNumber(handle);
        int_data _sub_prog_number = GetSubPrgNumber(handle);
        int_data _parts_count = GetPartsCount(handle);
        int_data _tool_number = GetToolNumber(handle);
        int_data _frame_number = GetFrameNum(handle);

        frame = _frame.data;
        main_prog_number = _main_prog_number.data;
        sub_prog_number = _sub_prog_number.data;
        parts_count = _parts_count.data;
        tool_number = _tool_number.data;
        frame_number = _frame_number.data;

        if (_frame.error)
            prg_err += " FRAME ERR: " + _frame.error_msg;
        if (_main_prog_number.error)
            prg_err += " MAIN PRG NUM ERR: " + _main_prog_number.error_msg;
        if (_sub_prog_number.error)
            prg_err += " SUB PRG NUM ERR: " + _sub_prog_number.error_msg;
        if (_parts_count.error)
            prg_err += " PARTS COUNT ERR: " + _parts_count.error_msg;
        if (_tool_number.error)
            prg_err += " TOOL NUMBER ERR: " + _tool_number.error_msg;
        if (_frame_number.error)
            prg_err += " FRAME NUMBER ERR: " + _frame_number.error_msg;
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
            {"frame_number", data.frame_number},
            {"prg_err", data.prg_err}
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
    std::string axes_err = "";

    AxesData(unsigned short handle) 
    {
        int_data _feedrate = GetFeedRate(handle);
        int_data _feed_override = GetFeedOverride(handle);
        float_data _jog_override = GetJogOverride(handle);
        int_data _jog_speed = GetJogSpeed(handle);
        float_data _current_load = GetServoCurrentLoad(handle);
        float_data _current_load_percent = GetServoCurrentPercentLoad(handle);
        map_data _servo_loads = GetAllServoLoad(handle);

        feedrate = _feedrate.data;
        feed_override = _feed_override.data;
        jog_override = _jog_override.data;
        jog_speed = _jog_speed.data;
        current_load = _current_load.data;
        current_load_percent = _current_load_percent.data;
        servo_loads = _servo_loads.data;

        if (_feedrate.error)
            axes_err += " FEEDRATE ERR: " + _feedrate.error_msg;
        if (_feed_override.error)
            axes_err += " FOV ERR: " + _feed_override.error_msg;
        if (_jog_override.error)
            axes_err += " JOV ERR: " + _jog_override.error_msg;
        if (_jog_speed.error)
            axes_err += " JOG SPEED ERR: " + _jog_speed.error_msg;
        if (_current_load.error)
            axes_err += " CURRENT LOAD ERR: " + _current_load.error_msg;
        if (_current_load_percent.error)
            axes_err += " CURRENT LOAD % ERR: " + _current_load_percent.error_msg;
        if (_servo_loads.error)
            axes_err += " SERVO LOAD ERR: " + _servo_loads.error_msg;
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
            {"servo_loads", data.servo_loads},
            {"axes_err", data.servo_loads}
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
    {}

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
            col_data.AddCollector(handle.data, device);
            if(!handle.error)
                FreeHandle(handle.data);
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