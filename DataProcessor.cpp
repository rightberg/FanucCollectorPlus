#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <ranges>
#include <map>
#include <functional>
#include "Fanuc.h"
#include "Collector.h"
#include <stdio.h>
#include <iostream>

struct MergedData 
{
    UShortData ushort_data;
    ShortData short_data;
    LongData long_data;
    DoubleData double_data;
    LongMapData long_map_data;
    DoubleMapData double_map_data;
    StrData str_data;
};

struct WriteContext
{
    unsigned short& handle;
    const Device& device;
    rapidjson::Writer<rapidjson::StringBuffer>& writer;
    MergedData& buf;
    std::map<std::string, int>& errors;
};

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

        if (device_value.HasMember("tags_pack") && device_value["tags_pack"].IsArray())
            for (const auto& v : device_value["tags_pack"].GetArray())
                if (v.IsString())
                    device.pack.push_back(v.GetString());

        if (device_value.HasMember("port") && device_value["port"].IsInt())
            device.port = device_value["port"].GetInt();

        if (device_value.HasMember("delay_ms") && device_value["delay_ms"].IsInt())
            device.delay_ms = device_value["delay_ms"].GetInt();

        devices.push_back(device);
    }
    return true;
}

static void WriteIntData(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::string_view key, const int& data)
{
    writer.Key(key.data());
    writer.Int(data);
}

static void WriteInt64Data(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::string_view key, const long& data)
{
    writer.Key(key.data());
    writer.Int64(data);
}

static void WriteDoubleData(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::string_view key, const double& data)
{
    writer.Key(key.data());
    writer.Double(data);
}

static void WriteStringData(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::string_view key, std::string_view data)
{
    writer.Key(key.data());
    writer.String(data.data());
}

static void WriteIntMapData(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::string_view key, const std::map<std::string, int>& data)
{
    writer.Key(key.data());
    writer.StartObject();
    for (const auto& pair : data)
    {
        writer.Key(pair.first.data());
        writer.Int(pair.second);
    }
    writer.EndObject();
}

static void WriteInt64MapData(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::string_view key,const std::map<std::string, long>& data)
{
    writer.Key(key.data());
    writer.StartObject();
    for (const auto& pair : data)
    {
        writer.Key(pair.first.data());
        writer.Int64(pair.second);
    }
    writer.EndObject();
}

static void WriteDoubleMapData(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::string_view key,const std::map<std::string, double>& data)
{
    writer.Key(key.data());
    writer.StartObject();
    for (const auto& pair : data)
    {
        writer.Key(pair.first.data());
        writer.Double(pair.second);
    }
    writer.EndObject();
}

std::map<std::string_view, std::function<void(WriteContext& ctx)>> fanuc_tags = {
    {"errors", [](WriteContext& ctx) {}},
    {"name", [](WriteContext& ctx) 
    {
        WriteStringData(ctx.writer, "name", ctx.device.name);
    }},
    {"address", [](WriteContext& ctx) 
    {
        WriteStringData(ctx.writer, "address", ctx.device.address);
    }},
    {"port", [](WriteContext& ctx) 
    {
        WriteIntData(ctx.writer, "port", ctx.device.port);
    }},
    {"aut", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetAut(ctx.handle);
        WriteIntData(ctx.writer, "aut", ctx.buf.short_data.data);
        ctx.errors["aut"] = ctx.buf.short_data.error;
    }},
    {"run", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetRun(ctx.handle);
        WriteIntData(ctx.writer, "run", ctx.buf.short_data.data);
        ctx.errors["run"] = ctx.buf.short_data.error;
    }},
    {"edit", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetEdit(ctx.handle);
        WriteIntData(ctx.writer, "edit", ctx.buf.short_data.data);
        ctx.errors["edit"] = ctx.buf.short_data.error;
    }},
    {"shutdowns", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetShutdowns(ctx.handle);
        WriteIntData(ctx.writer, "shutdowns", ctx.buf.short_data.data);
        ctx.errors["shutdowns"] = ctx.buf.short_data.error;
    }},
    {"g00", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetG00(ctx.handle);
        WriteIntData(ctx.writer, "g00", ctx.buf.short_data.data);
        ctx.errors["g00"] = ctx.buf.short_data.error;
    }},
    {"motion", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetMotion(ctx.handle);
        WriteIntData(ctx.writer, "motion", ctx.buf.short_data.data);
        ctx.errors["motion"] = ctx.buf.short_data.error;
    }},
    {"mstb", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetMstb(ctx.handle);
        WriteIntData(ctx.writer, "mstb", ctx.buf.short_data.data);
        ctx.errors["mstb"] = ctx.buf.short_data.error;
    }},
    {"load_excess", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetLoadExcess(ctx.handle);
        WriteIntData(ctx.writer, "load_excess", ctx.buf.short_data.data);
        ctx.errors["load_excess"] = ctx.buf.short_data.error;
    }},
    {"frame", [](WriteContext& ctx) 
    {
        ctx.buf.str_data = GetFrame(ctx.handle);
        WriteStringData(ctx.writer, "frame", ctx.buf.str_data.data);
        ctx.errors["frame"] = ctx.buf.str_data.error;
    }},
    {"main_prog_number", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetMainPrgNumber(ctx.handle);
        WriteIntData(ctx.writer, "main_prog_number", ctx.buf.short_data.data);
        ctx.errors["main_prog_number"] = ctx.buf.short_data.error;
    }},
    {"sub_prog_number", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetSubPrgNumber(ctx.handle);
        WriteIntData(ctx.writer, "sub_prog_number", ctx.buf.short_data.data);
        ctx.errors["sub_prog_number"] = ctx.buf.short_data.error;
    }},
    {"parts_count", [](WriteContext& ctx) 
    {
        ctx.buf.long_data = GetPartsCount(ctx.handle);
        WriteInt64Data(ctx.writer, "parts_count", ctx.buf.long_data.data);
        ctx.errors["parts_count"] = ctx.buf.long_data.error;
    }},
    {"tool_number", [](WriteContext& ctx) 
    {
        ctx.buf.long_data = GetToolNumber(ctx.handle);
        WriteInt64Data(ctx.writer, "tool_number", ctx.buf.long_data.data);
        ctx.errors["tool_number"] = ctx.buf.long_data.error;
    }},
    {"frame_number", [](WriteContext& ctx) 
    {
        ctx.buf.long_data = GetFrameNumber(ctx.handle);
        WriteInt64Data(ctx.writer, "frame_number", ctx.buf.long_data.data);
        ctx.errors["frame_number"] = ctx.buf.long_data.error;
    }},
    {"feedrate", [](WriteContext& ctx) 
    {
        ctx.buf.long_data = GetFeedRate(ctx.handle);
        WriteInt64Data(ctx.writer, "feedrate", ctx.buf.long_data.data);
        ctx.errors["feedrate"] = ctx.buf.long_data.error;
    }},
    {"feed_override", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetFeedOverride(ctx.handle);
        WriteIntData(ctx.writer, "feed_override", ctx.buf.short_data.data);
        ctx.errors["feed_override"] = ctx.buf.short_data.error;
    }},
    {"jog_override", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetJogOverride(ctx.handle);
        WriteIntData(ctx.writer, "jog_override", ctx.buf.short_data.data);
        ctx.errors["jog_override"] = ctx.buf.short_data.error;
    }},
    {"jog_speed", [](WriteContext& ctx) 
    {
        ctx.buf.double_map_data = GetJogSpeed(ctx.handle);
        WriteDoubleMapData(ctx.writer, "jog_speed", ctx.buf.double_map_data.data);
        ctx.errors["jog_speed"] = ctx.buf.double_map_data.error;
    }},
    {"current_load", [](WriteContext& ctx) 
    {
        ctx.buf.double_map_data = GetServoCurrentLoad(ctx.handle);
        WriteDoubleMapData(ctx.writer, "current_load", ctx.buf.double_map_data.data);
        ctx.errors["current_load"] = ctx.buf.double_map_data.error;
    }},
    {"current_load_percent", [](WriteContext& ctx) 
    {
        ctx.buf.long_map_data = GetServoCurrentPercentLoad(ctx.handle);
        WriteInt64MapData(ctx.writer, "current_load_percent", ctx.buf.long_map_data.data);
        ctx.errors["current_load_percent"] = ctx.buf.long_map_data.error;
    }},
    {"servo_loads", [](WriteContext& ctx) 
    {
        ctx.buf.long_map_data = GetAllServoLoad(ctx.handle);
        WriteInt64MapData(ctx.writer, "servo_loads", ctx.buf.long_map_data.data);
        ctx.errors["servo_loads"] = ctx.buf.long_map_data.error;
    }},
    {"absolute_positions", [](WriteContext& ctx) 
    {
        ctx.buf.double_map_data = GetAbsolutePositions(ctx.handle);
        WriteDoubleMapData(ctx.writer, "absolute_positions", ctx.buf.double_map_data.data);
        ctx.errors["absolute_positions"] = ctx.buf.double_map_data.error;
    }},
    {"machine_positions", [](WriteContext& ctx) 
    {
        ctx.buf.double_map_data = GetMachinePositions(ctx.handle);
        WriteDoubleMapData(ctx.writer, "machine_positions", ctx.buf.double_map_data.data);
        ctx.errors["machine_positions"] = ctx.buf.double_map_data.error;
    }},
    {"relative_positions", [](WriteContext& ctx) 
    {
        ctx.buf.double_map_data = GetRelativePositions(ctx.handle);
        WriteDoubleMapData(ctx.writer, "relative_positions", ctx.buf.double_map_data.data);
        ctx.errors["relative_positions"] = ctx.buf.double_map_data.error;
    }},
    {"spindle_speed", [](WriteContext& ctx) 
    {
        ctx.buf.double_data = GetSpindleSpeed(ctx.handle);
        WriteInt64Data(ctx.writer, "spindle_speed", ctx.buf.double_data.data);
        ctx.errors["spindle_speed"] = ctx.buf.double_data.error;
    }},
    {"spindle_param_speed", [](WriteContext& ctx) 
    {
        ctx.buf.long_map_data = GetSpindleSpeedParam(ctx.handle);
        WriteInt64MapData(ctx.writer, "spindle_param_speed", ctx.buf.long_map_data.data);
        ctx.errors["spindle_param_speed"] = ctx.buf.long_map_data.error;
    }},
    {"spindle_override", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetSpindleOverride(ctx.handle);
        WriteIntData(ctx.writer, "spindle_override", ctx.buf.short_data.data);
        ctx.errors["spindle_override"] = ctx.buf.short_data.error;
    }},
    {"spindle_motor_speed", [](WriteContext& ctx) 
    {
        ctx.buf.long_map_data = GetSpindleMotorSpeed(ctx.handle);
        WriteInt64MapData(ctx.writer, "spindle_motor_speed", ctx.buf.long_map_data.data);
        ctx.errors["spindle_motor_speed"] = ctx.buf.long_map_data.error;
    }},
    {"spindle_load", [](WriteContext& ctx) 
    {
        ctx.buf.long_map_data = GetSpindleLoad(ctx.handle);
        WriteInt64MapData(ctx.writer, "spindle_load", ctx.buf.long_map_data.data);
        ctx.errors["spindle_load"] = ctx.buf.long_map_data.error;
    }},
    {"emergency", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetEmergency(ctx.handle);
        WriteIntData(ctx.writer, "emergency", ctx.buf.short_data.data);
        ctx.errors["emergency"] = ctx.buf.short_data.error;
    }},
    {"alarm", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetAlarm(ctx.handle);
        WriteIntData(ctx.writer, "alarm", ctx.buf.short_data.data);
        ctx.errors["alarm"] = ctx.buf.short_data.error;
    }},
    {"axes_number", [](WriteContext& ctx) 
    {
        ctx.buf.short_data = GetCtrlAxesNumber(ctx.handle);
        WriteIntData(ctx.writer, "axes_number", ctx.buf.short_data.data);
        ctx.errors["axes_number"] = ctx.buf.short_data.error;
    }},
    {"spindles_number", [](WriteContext& ctx)
    {
        ctx.buf.short_data = GetCtrlSpindlesNumber(ctx.handle);
        WriteIntData(ctx.writer, "spindles_number", ctx.buf.short_data.data);
        ctx.errors["spindles_number"] = ctx.buf.short_data.error;
    }},
    {"channels_number", [](WriteContext& ctx)
    {
        ctx.buf.short_data = GetCtrlPathsNumber(ctx.handle);
        WriteIntData(ctx.writer, "channels_number", ctx.buf.short_data.data);
        ctx.errors["channels_number"] = ctx.buf.short_data.error;
    }},
    {"power_on_time", [](WriteContext& ctx)
    {
        ctx.buf.long_data = GetPowerOnTime(ctx.handle);
        WriteInt64Data(ctx.writer, "power_on_time", ctx.buf.long_data.data);
        ctx.errors["power_on_time"] = ctx.buf.long_data.error;
    }},
    {"operation_time", [](WriteContext& ctx)
    {
        ctx.buf.double_data = GetOperationTime(ctx.handle);
        WriteDoubleData(ctx.writer, "operation_time", ctx.buf.double_data.data);
        ctx.errors["operation_time"] = ctx.buf.double_data.error;
    }},
    {"cutting_time", [](WriteContext& ctx)
    {
        ctx.buf.double_data = GetCuttingTime(ctx.handle);
        WriteDoubleData(ctx.writer, "cutting_time", ctx.buf.double_data.data);
        ctx.errors["cutting_time"] = ctx.buf.double_data.error;
    }},
    {"cycle_time", [](WriteContext& ctx)
    {
        ctx.buf.double_data = GetCycleTime(ctx.handle);
        WriteDoubleData(ctx.writer, "cycle_time", ctx.buf.double_data.data);
        ctx.errors["cycle_time"] = ctx.buf.double_data.error;
    }},
    {"series_number", [](WriteContext& ctx)
    {
        ctx.buf.str_data = GetSeriesNumber(ctx.handle);
        WriteStringData(ctx.writer, "series_number", ctx.buf.str_data.data);
        ctx.errors["series_number"] = ctx.buf.str_data.error;
    }},
    {"version_number", [](WriteContext& ctx)
    {
        ctx.buf.str_data = GetVersionNumber(ctx.handle);
        WriteStringData(ctx.writer, "version_number", ctx.buf.str_data.data);
        ctx.errors["version_number"] = ctx.buf.str_data.error;
    }},
    { "serial_number", [](WriteContext& ctx)
    {
        ctx.buf.long_data = GetSerialNumber(ctx.handle);
        WriteInt64Data(ctx.writer, "serial_number", ctx.buf.long_data.data);
        ctx.errors["serial_number"] = ctx.buf.long_data.error;
    }},
    {"cnc_id", [](WriteContext& ctx)
    {
        ctx.buf.str_data = GetCncId(ctx.handle);
        WriteStringData(ctx.writer, "cnc_id", ctx.buf.str_data.data);
        ctx.errors["cnc_id"] = ctx.buf.str_data.error;
    }}
};

void InitTagPacks(std::vector<Device>& devices)
{
    std::vector<std::string> available_tags;
    for (const auto& [key, _] : fanuc_tags)
        available_tags.push_back(std::string(key));

    for (auto& device : devices)
    {
        if (device.pack.empty())
            device.pack = available_tags;
        else
        {
            std::vector<std::string> filtered{"name", "address", "port"};
            for (auto& tag : device.pack)
                if (std::find(available_tags.begin(), available_tags.end(), tag) != available_tags.end())
                    filtered.push_back(tag);
            device.pack = filtered;
        }
    }
}

void GetFanucDataJson(unsigned short& handle, const Device& device, std::string& json_data)
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    MergedData buf{};
    std::map<std::string, int> errors{};
    WriteContext ctx{handle, device, writer, buf, errors};

    writer.StartObject();
    for (const auto& key : device.pack)
        fanuc_tags[key](ctx);
    if (std::find(device.pack.begin(), device.pack.end(), "errors") != device.pack.end())
        WriteIntMapData(writer, "errors", errors);
    writer.EndObject();

    json_data = std::string(s.GetString());
}