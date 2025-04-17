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
    std::string name;
    std::string address;
    std::string series;
    int port;

    friend void to_json(nlohmann::json& j, const Device& d)
    {
        j = nlohmann::json
        { 
            {"name", d.name}, 
            {"address", d.address }, 
            {"port", d.port}, 
            {"series", d.series}
        };
    }

    static friend void from_json(const nlohmann::json& j, Device& d)
    {
        
        j.at("name").get_to(d.name);
        j.at("address").get_to(d.address);
        j.at("port").get_to(d.port);
        j.at("series").get_to(d.series);
    }
};

struct ModeData
{
    short mode = 0;
    short run_state = 0;
    short status = 0;
    short shutdowns = 0;
    short hight_speed = 0;
    short axis_motion = 0;
    short mstb = 0;
    long load_excess = 0;

    std::string mode_str = "";
    std::string run_state_str = "";
    std::string status_str = "";
    std::string shutdowns_str = "";
    std::string hight_speed_str = "";
    std::string axis_motion_str = "";
    std::string mstb_str = "";
    std::string load_excess_str = "";

    std::vector<short> mode_errors = std::vector<short>(8);
    std::vector<std::string> mode_errors_str = std::vector<std::string>(8);

    ModeData(unsigned short handle, const std::string& series)
    {
        short_data _mode = GetMode(handle, series);
        short_data _run_state = GetRunState(handle, series);
        short_data _status = GetStatus(handle, series);
        short_data _shutdowns = GetShutdowns(handle);
        short_data _hight_speed = GetHightSpeed(handle);
        short_data _axis_motion = GetAxisMotion(handle, series);
        short_data _mstb = GetMstb(handle, series);
        long_data _load_excess = GetLoadExcess(handle, series);

        if (_mode.IsError())
        {
            mode_errors[0] = _mode.error;
            mode_errors_str[0] = GetCncErrorMessage(_mode.error);
        }
        else
        {
            mode = _mode.data;
            mode_str = GetModeString(mode, series);
        }

        if (_run_state.IsError())
        {
            mode_errors[1] = _run_state.error;
            mode_errors_str[1] = GetCncErrorMessage(_run_state.error);
        }
        else
        {
            run_state = _run_state.data;
            run_state_str = GetRunStateString(run_state, series);
        }

        if (_status.IsError())
        {
            mode_errors[2] = _status.error;
            mode_errors_str[2] = GetCncErrorMessage(_status.error);
        }
        else
        {
            status = _status.data;
            status_str = GetStatusString(status);
        }

        if (_shutdowns.IsError())
        {
            mode_errors[3] = _shutdowns.error;
            mode_errors_str[3] = GetCncErrorMessage(_shutdowns.error);
        }
        else
        {
            shutdowns = _shutdowns.data;
            shutdowns_str = GetShutdownsString(shutdowns);
        }

        if (_hight_speed.IsError())
        {
            mode_errors[4] = _hight_speed.error;
            mode_errors_str[4] = GetCncErrorMessage(_hight_speed.error);
        }
        else
        {
            hight_speed = _hight_speed.data;
            hight_speed_str = GetHightSpeedString(hight_speed);
        }

        if (_axis_motion.IsError())
        {
            mode_errors[5] = _axis_motion.error;
            mode_errors_str[5] = GetCncErrorMessage(_axis_motion.error);
        }
        else
        {
            axis_motion = _axis_motion.data;
            axis_motion_str = GetAxisMotionString(axis_motion, series);
        }

        if (_mstb.IsError())
        {
            mode_errors[6] = _mstb.error;
            mode_errors_str[6] = GetCncErrorMessage(_mstb.error);
        }
        else
        {
            mstb = _mstb.data;
            mstb_str = GetMstbString(mstb, series);
        }

        if (_load_excess.IsError())
        {
            mode_errors[7] = _load_excess.error;
            mode_errors_str[7] = GetCncErrorMessage(_load_excess.error);
        }
        else
        {
            load_excess = _load_excess.data;
            load_excess_str = GetLoadExcessString(load_excess, series);
        }

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
            {"mode_str", data.mode_str},
            {"run_state_str", data.run_state_str},
            {"status_str", data.status_str},
            {"shutdowns_str", data.shutdowns_str},
            {"hight_speed_str", data.hight_speed_str},
            {"axis_motion_str", data.axis_motion_str},
            {"mstb_str", data.mstb_str},
            {"load_excess_str", data.load_excess_str},
            {"mode_errors", data.mode_errors},
            {"mode_errors_str", data.mode_errors_str}
        };
    };
};

struct ProgramData
{
    std::string frame = "";
    short main_prog_number = 0;
    short sub_prog_number = 0;
    int parts_count = 0;
    long tool_number = 0;
    long frame_number = 0;

    std::vector<short> program_errors = std::vector<short>(6);
    std::vector<std::string> program_errors_str = std::vector<std::string>(6);

    ProgramData(unsigned short handle)
    {
        str_data _frame = GetFrame(handle);
        short_data _main_prog_number = GetMainPrgNumber(handle);
        short_data _sub_prog_number = GetSubPrgNumber(handle);
        int_data _parts_count = GetPartsCount(handle);
        long_data _tool_number = GetToolNumber(handle);
        long_data _frame_number = GetFrameNumber(handle);

        if (_frame.IsError())
        {
            program_errors[0] = _frame.error;
            program_errors_str[0] = GetCncErrorMessage(_frame.error);
        }
        else
            frame = _frame.data;

        if (_main_prog_number.IsError())
        {
            program_errors[1] = _main_prog_number.error;
            program_errors_str[1] = GetCncErrorMessage(_main_prog_number.error);
        }
        else
            main_prog_number = _main_prog_number.data;

        if (_sub_prog_number.IsError())
        {
            program_errors[2] = _sub_prog_number.error;
            program_errors_str[2] = GetCncErrorMessage(_sub_prog_number.error);
        }
        else
            sub_prog_number = _sub_prog_number.data;

        if (_parts_count.IsError())
        {
            program_errors[3] = _parts_count.error;
            program_errors_str[3] = GetCncErrorMessage(_parts_count.error);
        }
        else
            parts_count = _parts_count.data;

        if (_tool_number.IsError())
        {
            program_errors[4] = _tool_number.error;
            program_errors_str[4] = GetCncErrorMessage(_tool_number.error);
        }
        else
            tool_number = _tool_number.data;

        if (_frame_number.IsError())
        {
            program_errors[5] = _frame_number.error;
            program_errors_str[5] = GetCncErrorMessage(_frame_number.error);
        }
        else
            frame_number = _frame_number.data;
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
            {"program_errors", data.program_errors},
            {"program_errors_str", data.program_errors_str}
        };
    };
};

struct AxesData
{
    long feedrate = 0;
    int feed_override = 0;
    float jog_override = 0;
    long jog_speed = 0;
    float current_load = 0;
    float current_load_percent = 0;
    std::map<std::string, int> servo_loads = {};

    std::vector<short> axes_errors = std::vector<short>(7);
    std::vector<std::string> axes_errors_str = std::vector<std::string>(7);

    AxesData(unsigned short handle) 
    {
        long_data _feedrate = GetFeedRate(handle);
        short_data _feed_override = GetFeedOverride(handle);
        short_data _jog_override = GetJogOverride(handle);
        long_data _jog_speed = GetJogSpeed(handle);
        float_data _current_load = GetServoCurrentLoad(handle);
        float_data _current_load_percent = GetServoCurrentPercentLoad(handle);
        map_data _servo_loads = GetAllServoLoad(handle);

        if (_feedrate.IsError())
        {
            axes_errors[0] = _feedrate.error;
            axes_errors_str[0] = GetCncErrorMessage(_feedrate.error);
        }
        else
            feedrate = _feedrate.data;


        if (_feed_override.IsError())
        {
            axes_errors[1] = _feed_override.error;
            axes_errors_str[1] = GetCncErrorMessage(_feed_override.error);
        }
        else
            feed_override = _feed_override.data;


        if (_jog_override.IsError())
        {
            axes_errors[2] = _jog_override.error;
            axes_errors_str[2] = GetCncErrorMessage(_jog_override.error);
        }
        else
            jog_override = _jog_override.data;


        if (_jog_speed.IsError())
        {
            axes_errors[3] = _jog_speed.error;
            axes_errors_str[3] = GetCncErrorMessage(_jog_speed.error);
        }
        else
            jog_speed = _jog_speed.data;

        if (_current_load.IsError())
        {
            axes_errors[4] = _current_load.error;
            axes_errors_str[4] = GetCncErrorMessage(_current_load.error);
        }
        else
            current_load = _current_load.data;

        if (_current_load_percent.IsError())
        {
            axes_errors[5] = _current_load_percent.error;
            axes_errors_str[5] = GetCncErrorMessage(_current_load_percent.error);
        }
        else
            current_load_percent = _current_load_percent.data;

        if (_servo_loads.IsError())
        {
            axes_errors[6] = _servo_loads.error;
            axes_errors_str[6] = GetCncErrorMessage(_servo_loads.error);
        }
        else
            servo_loads = _servo_loads.data;
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
            {"axes_errors", data.axes_errors},
            {"axes_errors_str", data.axes_errors_str}
        };
    }
};

struct SpindleData
{
    long spindle_speed = 0;
    long spindle_param_speed = 0;
    std::map<std::string, int> spindle_motor_speed = {};
    std::map<std::string, int> spindle_load = {};
    short spindle_override = 0;

    std::vector<short> spindle_errors = std::vector<short>(5);
    std::vector<std::string> spindle_errors_str = std::vector<std::string>(5);

    SpindleData(unsigned short handle)
    {
        long_data _spindle_speed = GetSpindleSpeed(handle);
        long_data _spindle_param_speed = GetSpindleSpeedParam(handle);
        map_data _spindle_motor_speed = GetSpindleMotorSpeed(handle);
        map_data _spindle_load = GetSpindleLoad(handle);
        short_data _spindle_override = GetSpindleOverride(handle);

        if (_spindle_speed.IsError())
        {
            spindle_errors[0] = _spindle_speed.error;
            spindle_errors_str[0] = GetCncErrorMessage(_spindle_speed.error);
        }
        else
            spindle_speed = _spindle_speed.data;

        if (_spindle_param_speed.IsError())
        {
            spindle_errors[1] = _spindle_param_speed.error;
            spindle_errors_str[1] = GetCncErrorMessage(_spindle_param_speed.error);
        }
        else
            spindle_param_speed = _spindle_param_speed.data;

        if (_spindle_motor_speed.IsError())
        {
            spindle_errors[2] = _spindle_motor_speed.error;
            spindle_errors_str[2] = GetCncErrorMessage(_spindle_motor_speed.error);
        }
        else
            spindle_motor_speed = _spindle_motor_speed.data;

        if (_spindle_load.IsError())
        {
            spindle_errors[3] = _spindle_load.error;
            spindle_errors_str[3] = GetCncErrorMessage(_spindle_load.error);
        }
        else
            spindle_load = _spindle_load.data;

        if (_spindle_override.IsError())
        {
            spindle_errors[4] = _spindle_override.error;
            spindle_errors_str[4] = GetCncErrorMessage(_spindle_override.error);
        }
        else
            spindle_override = _spindle_override.data;
    }

    static friend void to_json(nlohmann::json& j, const SpindleData& data)
    {
        j = nlohmann::json
        {
            {"spindle_speed", data.spindle_speed},
            {"spindle_param_speed", data.spindle_param_speed},
            {"spindle_motor_speed", data.spindle_motor_speed},
            {"spindle_load", data.spindle_load},
            {"spindle_override", data.spindle_override},
            {"spindle_errors", data.spindle_errors},
            {"spindle_errors_str", data.spindle_errors_str}
        };
    }
};

struct AlarmData
{
    short emergency = 0;
    short alarm_status = 0;
    std::string emergency_str = "";
    std::string alarm_status_str = "";

    std::vector<short> alarm_errors = std::vector<short>(2);
    std::vector<std::string> alarm_errors_str = std::vector<std::string>(2);

    AlarmData(unsigned short handle, const std::string& series)
    {
        short_data _emergency = GetEmergencyStop(handle, series);
        short_data _alarm_status = GetAlarmStatus(handle, series);

        if (_emergency.IsError())
        {
            alarm_errors[0] = _emergency.error;
            alarm_errors_str[0] = GetCncErrorMessage(_emergency.error);
        }
        else
        {
            emergency = _emergency.data;
            emergency_str = GetEmergencyString(emergency, series);
        }

        if (_alarm_status.IsError())
        {
            alarm_errors[1] = _alarm_status.error;
            alarm_errors_str[1] = GetCncErrorMessage(_alarm_status.error);
        }
        else
        {
            alarm_status = _alarm_status.data;
            alarm_status_str = GetAlarmStatusString(alarm_status, series);
        }
    }

    static friend void to_json(nlohmann::json& j, const AlarmData& data)
    {
        j = nlohmann::json
        {
            {"emergency", data.emergency},
            {"alarm_status", data.alarm_status},
            {"emergency_str", data.emergency_str},
            {"alarm_status_str", data.alarm_status_str},
            {"alarm_errors", data.alarm_errors},
            {"alarm_errors_str", data.alarm_errors_str}
        };
    }
};

struct Collector
{
    Device device;
    ModeData mode_data;
    ProgramData program_data;
    AxesData axes_data;
    SpindleData spindle_data;
    AlarmData alarm_data;

    Collector(unsigned short handle, const Device device) :
        device(device),
        mode_data(handle, device.series),
        program_data(handle),
        axes_data(handle),
        spindle_data(handle),
        alarm_data(handle, device.series)
    {}

    static friend void to_json(nlohmann::json& j, const Collector& collector)
    {
        j = nlohmann::json
        {
            {"device", collector.device},
            {"mode_data", collector.mode_data},
            {"program_data", collector.program_data},
            {"axes_data", collector.axes_data},
            {"spindle_data", collector.spindle_data},
            {"alarm_data", collector.alarm_data}
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
            ushort_data handle = GetHandle(device.address, device.port, 5);
            col_data.AddCollector(handle.data, device);
            if(!handle.IsError())
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