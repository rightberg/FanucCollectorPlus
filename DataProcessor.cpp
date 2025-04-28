#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "Fanuc.h"
#include "FanucTypes.h"
#include "Collector.h"

bool SetFanucData(unsigned short handle, const Device& device, FanucData& data)
{
    //mode data
    short_data aut = GetMode(handle);
    short_data run = GetRunState(handle);
    short_data edit = GetStatus(handle);
    short_data shutdowns = GetShutdowns(handle);
    short_data hight_speed = GetHightSpeed(handle);
    short_data motion = GetAxisMotion(handle);
    short_data mstb = GetMstb(handle);
    long_data load_excess = GetLoadExcess(handle);
    //program data
    str_data frame = GetFrame(handle);
    short_data main_prog_number = GetMainPrgNumber(handle);
    short_data sub_prog_number = GetSubPrgNumber(handle);
    int_data parts_count = GetPartsCount(handle);
    long_data tool_number = GetToolNumber(handle);
    long_data frame_number = GetFrameNumber(handle);
    //axes data
    long_data feedrate = GetFeedRate(handle);
    short_data feed_override = GetFeedOverride(handle);
    short_data jog_override = GetJogOverride(handle);
    map_data servo_loads = GetAllServoLoad(handle);
    long_map_data absolute_positions = GetAbsolutePositions(handle);
    long_map_data machine_positions = GetMachinePositions(handle);
    long_map_data relative_positions = GetRelativePositions(handle);
    long_map_data current_load_percent = GetServoCurrentPercentLoad(handle);
    double_map_data current_load = GetServoCurrentLoad(handle);
    double_map_data jog_speed = GetJogSpeed(handle);
    //spindle data
    long_data spindle_speed = GetSpindleSpeed(handle);
    map_data spindle_motor_speed = GetSpindleMotorSpeed(handle);
    map_data spindle_load = GetSpindleLoad(handle);
    short_data spindle_override = GetSpindleOverride(handle);
    long_map_data spindle_param_speed = GetSpindleSpeedParam(handle);
    //alarm data
    short_data emergency = GetEmergencyStop(handle);
    short_data alarm = GetAlarmStatus(handle);
    //other data
    long_data power_on_time = GetPowerOnTime(handle);
    long_data operation_time = GetOperationTime(handle);
    long_data cutting_time = GetCuttingTime(handle);
    long_data cycle_time = GetCycleTime(handle);
    str_data series_number = GetSeriesNumber(handle);
    str_data version_number = GetVersionNumber(handle);

    //pull device data
    data.name = device.name;
    data.address = device.address;
    data.port = device.port;
    data.series = device.series;
    //pull mode data
    aut.PullData(data.aut, data.errors[0]);
    run.PullData(data.run, data.errors[1]);
    edit.PullData(data.edit, data.errors[2]);
    shutdowns.PullData(data.shutdowns, data.errors[3]);
    hight_speed.PullData(data.hight_speed, data.errors[4]);
    motion.PullData(data.motion, data.errors[5]);
    mstb.PullData(data.mstb, data.errors[6]);
    load_excess.PullData(data.load_excess, data.errors[7]);
    //pull program data
    frame.PullData(data.frame, data.errors[8]);
    main_prog_number.PullData(data.main_prog_number, data.errors[9]);
    sub_prog_number.PullData(data.sub_prog_number, data.errors[10]);
    parts_count.PullData(data.parts_count, data.errors[11]);
    tool_number.PullData(data.tool_number, data.errors[12]);
    frame_number.PullData(data.frame_number, data.errors[13]);
    //pull axes data
    feedrate.PullData(data.feedrate, data.errors[14]);
    feed_override.PullData(data.feed_override, data.errors[15]);
    jog_override.PullData(data.jog_override, data.errors[16]);
    jog_speed.PullData(data.jog_speed, data.errors[17]);
    current_load.PullData(data.current_load, data.errors[18]);
    current_load_percent.PullData(data.current_load_percent, data.errors[19]);
    servo_loads.PullData(data.servo_loads, data.errors[20]);
    absolute_positions.PullData(data.absolute_positions, data.errors[21]);
    machine_positions.PullData(data.machine_positions, data.errors[22]);
    relative_positions.PullData(data.relative_positions, data.errors[23]);
    //pull spindle data
    spindle_speed.PullData(data.spindle_speed, data.errors[24]);
    spindle_param_speed.PullData(data.spindle_param_speed, data.errors[25]);
    spindle_motor_speed.PullData(data.spindle_motor_speed, data.errors[26]);
    spindle_load.PullData(data.spindle_load, data.errors[27]);
    spindle_override.PullData(data.spindle_override, data.errors[28]);
    //pull alarm data
    emergency.PullData(data.emergency, data.errors[29]);
    alarm.PullData(data.alarm, data.errors[30]);
    //pull other data
    power_on_time.PullData(data.power_on_time, data.errors[31]);
    operation_time.PullData(data.operation_time, data.errors[32]);
    cutting_time.PullData(data.cutting_time, data.errors[33]);
    cycle_time.PullData(data.cycle_time, data.errors[34]);
    series_number.PullData(data.series_number, data.errors[35]);
    version_number.PullData(data.version_number, data.errors[36]);

    int equal_counter = 0;
    short first_error = data.errors[0];
    int max_errors = data.errors.size();
    for (int i = 0; i < max_errors; i++)
    {
        if (data.errors[i] != 0)
        {
            if (first_error == data.errors[i])
                equal_counter++;
        }
    }
    if (equal_counter == max_errors)
        return false;
    else
        return true;
}

bool ParseDevices(const char* json, std::vector<Device>& devices)
{
    rapidjson::Document doc;
    rapidjson::ParseResult ok = doc.Parse(json);

    if (!ok)
        return false;

    for (const auto& device_value : doc.GetArray()) 
    {
        Device device;
        if (device_value.HasMember("name") && device_value["name"].IsString())
            device.name = device_value["name"].GetString();

        if (device_value.HasMember("address") && device_value["address"].IsString())
            device.address = device_value["address"].GetString();

        if (device_value.HasMember("series") && device_value["series"].IsString())
            device.series = device_value["series"].GetString();

        if (device_value.HasMember("port") && device_value["port"].IsInt())
            device.port = device_value["port"].GetInt();

        if (device_value.HasMember("delay_ms") && device_value["delay_ms"].IsInt())
            device.delay_ms = device_value["delay_ms"].GetInt();

        devices.push_back(device);
    }
    return true;
}

std::string SerializeFanucData(FanucData& data)
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    writer.StartObject();

    //device data
    writer.Key("name");    writer.String(data.name.c_str());
    writer.Key("series");  writer.String(data.series.c_str());
    writer.Key("address"); writer.String(data.address.c_str());
    writer.Key("port");    writer.Int(data.port);
    //mode data 
    writer.Key("aut");        writer.Int(data.aut);
    writer.Key("run");   writer.Int(data.run);
    writer.Key("edit");      writer.Int(data.edit);
    writer.Key("shutdowns");   writer.Int(data.shutdowns);
    writer.Key("hight_speed"); writer.Int(data.hight_speed);
    writer.Key("motion"); writer.Int(data.motion);
    writer.Key("mstb");        writer.Int(data.mstb);
    writer.Key("load_excess"); writer.Int64(data.load_excess);
    //program data
    writer.Key("frame");            writer.String(data.frame.c_str());
    writer.Key("main_prog_number"); writer.Int(data.main_prog_number);
    writer.Key("sub_prog_number");  writer.Int(data.sub_prog_number);
    writer.Key("parts_count");      writer.Int(data.parts_count);
    writer.Key("tool_number");      writer.Int64(data.tool_number);
    writer.Key("frame_number");     writer.Int64(data.frame_number);
    //axes data
    writer.Key("feedrate");             writer.Int64(data.feedrate);
    writer.Key("feed_override");        writer.Int(data.feed_override);
    writer.Key("jog_override");         writer.Int(data.jog_override);

    writer.Key("current_load_percent");
    writer.StartObject();
    for (const auto& pair : data.current_load_percent)
    {
        writer.Key(pair.first.c_str());
        writer.Int64(pair.second);
    }
    writer.EndObject();

    writer.Key("current_load");
    writer.StartObject();
    for (const auto& pair : data.current_load)
    {
        writer.Key(pair.first.c_str());
        writer.Double(pair.second);
    }
    writer.EndObject();

    writer.Key("jog_speed");
    writer.StartObject();
    for (const auto& pair : data.jog_speed)
    {
        writer.Key(pair.first.c_str());
        writer.Double(pair.second);
    }
    writer.EndObject();
    writer.Key("servo_loads");
    writer.StartObject();
    for (const auto& pair : data.servo_loads)
    {
        writer.Key(pair.first.c_str());
        writer.Int(pair.second);
    }
    writer.EndObject();
    writer.Key("absolute_positions");
    writer.StartObject();
    for (const auto& pair : data.absolute_positions)
    {
        writer.Key(pair.first.c_str());
        writer.Int(pair.second);
    }
    writer.EndObject();
    writer.Key("machine_positions");
    writer.StartObject();
    for (const auto& pair : data.machine_positions)
    {
        writer.Key(pair.first.c_str());
        writer.Int(pair.second);
    }
    writer.EndObject();
    writer.Key("relative_positions");
    writer.StartObject();
    for (const auto& pair : data.relative_positions)
    {
        writer.Key(pair.first.c_str());
        writer.Int(pair.second);
    }
    writer.EndObject();
    //spindle data
    writer.Key("spindle_speed");       writer.Int64(data.spindle_speed);
    writer.Key("spindle_override");    writer.Int(data.spindle_override);

    writer.Key("spindle_param_speed");
    writer.StartObject();
    for (const auto& pair : data.spindle_param_speed)
    {
        writer.Key(pair.first.c_str());
        writer.Int64(pair.second);
    }
    writer.EndObject();

    writer.Key("spindle_motor_speed");
    writer.StartObject();
    for (const auto& pair : data.spindle_motor_speed)
    {
        writer.Key(pair.first.c_str());
        writer.Int(pair.second);
    }
    writer.EndObject();

    writer.Key("spindle_load");
    writer.StartObject();
    for (const auto& pair : data.spindle_load)
    {
        writer.Key(pair.first.c_str());
        writer.Int(pair.second);
    }
    writer.EndObject();
    //alarm data
    writer.Key("emergency");    writer.Int(data.emergency);
    writer.Key("alarm"); writer.Int(data.alarm);
    //other data
    writer.Key("power_on_time");    writer.Int(data.power_on_time);
    writer.Key("operation_time"); writer.Int(data.operation_time);
    writer.Key("cutting_time");    writer.Int(data.cutting_time);
    writer.Key("cycle_time"); writer.Int(data.cycle_time);
    writer.Key("series_number"); writer.String(data.series_number.c_str());
    writer.Key("version_number"); writer.String(data.version_number.c_str());
    //errors data
    writer.Key("errors");
    writer.StartArray();
    for (auto error : data.errors)
        writer.Int(error);
    writer.EndArray();

    writer.EndObject();
    return std::string(s.GetString());
}