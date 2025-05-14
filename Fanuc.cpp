#pragma warning(disable: 4996)
#pragma comment(lib, "focas/fwlib32.lib")
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "focas/fwlib32.h"
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <format>
#include "FanucTypes.h"
#define _CRT_SECURE_NO_WARNINGS

#pragma region Handle functions

UShortData GetHandle(std::string& address, int& port, int& timeout)
{
    UShortData res = {};
    short ret = cnc_allclibhndl3(address.c_str(), port, timeout, &res.data);
    if (ret != EW_OK)
        res.error = ret;
    return res;
}

VoidFunc FreeHandle(unsigned short& handle)
{
    VoidFunc res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        short ret = cnc_freelibhndl(handle);
        if (ret != EW_OK)
            res.error = ret;
    }
    return res;
}

#pragma endregion

#pragma region Mode data

ShortData GetMode(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBST buf;
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.aut;
    }
    return res;
}

ShortData GetRunState(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBST buf;
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.run;
    }
    return res;
}

ShortData GetStatus(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBST buf;
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.edit;
    }
    return res;
}

ShortData GetShutdowns(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        unsigned short length = 256;
        short blknum;
        char buf[256];
        short ret = cnc_rdexecprog(handle, &length, &blknum, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            const char* commands[] = { "M00", "M01", "G04" };
            const int commands_count = sizeof(commands) / sizeof(commands[0]);
            for (int i = 0; i < commands_count; i++)
                if (strstr(buf, commands[i]))
                    res.data = i;
            res.data = 3;
        }
    }
    return res;
}

ShortData GetG00(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBMDL buf;
        short ret = cnc_modal(handle, 0, 0, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            if (buf.modal.g_data == 0)
                res.data = 1;
            else
                res.data = 0;
        }
    }
    return res;
}

ShortData GetAxisMotion(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBST buf;
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.motion;
    }
    return res;
}

ShortData GetMstb(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBST buf;
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.mstb;
    }
    return res;
}

ShortData GetLoadExcess(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        short num = MAX_AXIS;
        ODBSVLOAD buf_1[MAX_AXIS] = {};
        short ret = cnc_rdsvmeter(handle, &num, buf_1);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            short dec = 0;
            double value = 0;
            for (int i = 0; i < num; i++)
            {
                dec = buf_1[i].svload.dec;
                value = buf_1[i].svload.data * std::pow(10, -dec);
                if (value > 100)

                    res.data = 1;
                break;
            }
            num = MAX_SPINDLE;
            ODBSPLOAD buf_2[MAX_SPINDLE] = {};
            ret = cnc_rdspmeter(handle, 0, &num, buf_2);
            if (ret != EW_OK)
                res.error = ret;
            else
            {
                for (int i = 0; i < num; i++)
                {
                    dec = buf_2[i].spload.dec;
                    value = buf_2[i].spload.data * std::pow(10, -dec);
                    if (value > 100)
                    {
                        if (res.data == 0)
                            res.data = 2;
                        else
                            res.data = 3;
                        break;
                    }
                }
            }
        }
    }
    return res;
}

#pragma endregion

#pragma region Program data

ShortData GetMainPrgNumber(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBPRO buf = {};
        short ret = cnc_rdprgnum(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.mdata;
    }
    return res;
}

ShortData GetSubPrgNumber(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBPRO buf = {};
        short ret = cnc_rdprgnum(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.data;
    }
    return res;
}

LongData GetFrameNumber(unsigned short& handle)
{
    LongData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSEQ buf = {};
        short ret = cnc_rdseqnum(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.data;
    }
    return res;
}

StrData GetFrame(unsigned short& handle)
{
    StrData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        char buf[1024];
        unsigned short length = sizeof(buf);
        short num;
        short ret = cnc_rdexecprog(handle, &length, &num, buf);
        if (ret != EW_OK)
            res.error = ret;
        else if (length != 0 && num >= 0 && length < sizeof(buf))
        {
            LongData frame_num_data = GetFrameNumber(handle);
            if (frame_num_data.IsError())
                res.error = ret;
            else
            {
                buf[length] = '\0';
                std::vector<std::string> lines;
                std::string line;
                for (int i = 0; i < length; ++i)
                {
                    if (buf[i] == '\n' || buf[i] == '\0')
                    {
                        lines.push_back(line);
                        line.clear();
                    }
                    else
                        line.push_back(buf[i]);
                }
                if (!line.empty())
                    lines.push_back(line);

                for (const auto& line : lines)
                    if (line.find("N" + std::to_string(frame_num_data.data)) != std::string::npos)
                        res.data = line;

                if (!lines.empty())
                    res.data = lines.front();
            }
        }
    }
    return res;
}

LongData GetPartsCount(unsigned short& handle)
{
    LongData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBPSD buf;
        short ret = cnc_rdparam(handle, 6711, -1, sizeof(buf), &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.u.rdata.prm_val;
    }
    return res;
}

LongData GetToolNumber(unsigned short& handle)
{
    LongData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBTLIFE4 buf = {};
        short ret = cnc_toolnum(handle, 0, 0, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.data;
    }
    return res;
}
#pragma endregion

#pragma region Axis data

DoubleMapData GetAbsolutePositions(unsigned short& handle)
{
    DoubleMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBPOS pos[MAX_AXIS];
        short num = MAX_AXIS;
        short ret = cnc_rdposition(handle, 0, &num, pos);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            short dec;
            for (int i = 0; i < num; i++)
            {
                std::string name(1, pos[i].abs.name);
                dec = pos[i].abs.dec;
                res.data[name] = pos[i].abs.data * std::pow(10,-dec);
            }
        }
    }
    return res;
}

DoubleMapData GetMachinePositions(unsigned short& handle)
{
    DoubleMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBPOS pos[MAX_AXIS];
        short num = MAX_AXIS;
        short ret = cnc_rdposition(handle, 1, &num, pos);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            short dec;
            for (int i = 0; i < num; i++)
            {
                std::string name(1, pos[i].mach.name);
                dec = pos[i].mach.dec;
                res.data[name] = pos[i].mach.data * std::pow(10, -dec);
            }
        }
    }
    return res;
}

DoubleMapData GetRelativePositions(unsigned short& handle)
{
    DoubleMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBPOS pos[MAX_AXIS];
        short num = MAX_AXIS;
        short ret = cnc_rdposition(handle, 2, &num, pos);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            short dec;
            for (int i = 0; i < num; i++)
            {
                std::string name(1, pos[i].rel.name);
                dec = pos[i].rel.dec;
                res.data[name] = pos[i].rel.data * std::pow(10, -dec);
            }
        }
    }
    return res;   
}

LongData GetFeedRate(unsigned short& handle)
{
    LongData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBACT buf = {};
        short ret = cnc_actf(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.data;
    }
    return res;
}

ShortData GetFeedOverride(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBSGNL buf = {};
        short ret = cnc_rdopnlsgnl(handle, 0x20, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.feed_ovrd;
    }
    return res;
}

ShortData GetJogOverride(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBSGNL buf = {};
        short ret = cnc_rdopnlsgnl(handle, 0x20, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.jog_ovrd;
    }
    return res;
}

DoubleMapData GetJogSpeed(unsigned short& handle)
{
    DoubleMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBAXDT buf[MAX_AXIS];
        short types[1] = { 2 };
        short lenght = MAX_AXIS;
        short ret = cnc_rdaxisdata(handle, 5, types, 1, &lenght, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            int flag = 0;
            long data = 0;
            short dec = 0;
            for (int i = 0; i < lenght; i++) 
            {
                flag = (buf[i].flag >> 1) & 1;
                if (flag == 0)
                {
                    dec = buf[i].dec;
                    data = buf[i].data;
                    res.data[buf[i].name] = data * std::pow(10, -dec);
                }
            }
        }
    }
    return res;
}

LongMapData GetAllServoLoad(unsigned short& handle)
{
    LongMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        short num = MAX_AXIS;
        ODBSVLOAD buf[MAX_AXIS] = {};
        short ret = cnc_rdsvmeter(handle, &num, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            for (int i = 0; i < num; i++)
            {
                std::string name = std::string(1, buf[i].svload.name);
                int value = buf[i].svload.data;
                res.data[name] = value;
            }
        }
    }
    return res;
}

DoubleMapData GetServoCurrentLoad(unsigned short& handle)
{
    DoubleMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBAXDT buf[MAX_AXIS];
        short types[1] = { 2 };
        short lenght = MAX_AXIS;
        short ret = cnc_rdaxisdata(handle, 2, types, 1, &lenght, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            long data = 0;
            short dec = 0;
            for (int i = 0; i < lenght; i++)
            {
                dec = buf[i].dec;
                data = buf[i].data;
                res.data[buf[i].name] = data * std::pow(10, -dec);
            }
        }
    }
    return res;
}

LongMapData GetServoCurrentPercentLoad(unsigned short& handle)
{
    LongMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBAXDT buf[MAX_AXIS];
        short types[1] = { 1 };
        short lenght = MAX_AXIS;
        short ret = cnc_rdaxisdata(handle, 2, types, 1, &lenght, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            for (int i = 0; i < lenght; i++)
                res.data[buf[i].name] = buf[i].data;
    }
    return res;
}
#pragma endregion

#pragma region Spindle data

LongData GetSpindleSpeed(unsigned short& handle)
{
    LongData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSPEED buf;
        short type = 1;
        short ret = cnc_rdspeed(handle, type, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.acts.data;
    }
    return res;
}

LongMapData GetSpindleSpeedParam(unsigned short& handle)
{
    LongMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBAXDT buf[MAX_SPINDLE];
        short types[1] = { 2 };
        short lenght = MAX_SPINDLE;
        short ret = cnc_rdaxisdata(handle, 3, types, 1, &lenght, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            for (int i = 0; i < lenght; i++)
                res.data[buf[i].name] = buf[i].data;
    }
    return res;
}

LongMapData GetSpindleMotorSpeed(unsigned short& handle)
{
    LongMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSPLOAD buf[MAX_SPINDLE];
        short num = MAX_SPINDLE;
        short ret = cnc_rdspmeter(handle, 1, &num, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            std::string name;
            for (int i = 0; i < num; i++)
            {
                name.clear();
                name += buf[i].spspeed.name;
                name += buf[i].spspeed.suff1;
                res.data[name] = buf[i].spspeed.data;
            }
        }
    }
    return res;
}

LongMapData GetSpindleLoad(unsigned short& handle)
{
    LongMapData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSPLOAD buf[MAX_SPINDLE];
        short num = MAX_SPINDLE;
        short ret = cnc_rdspmeter(handle, 0, &num, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            std::string name;
            for (int i = 0; i < num; i++)
            {
                name.clear();
                name += buf[i].spload.name;
                name += buf[i].spload.suff1;
                res.data[name] = buf[i].spload.data;
            }
        }
    }
    return res;
}

// only 15i series
ShortData GetSpindleOverride(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBSGNL buf;
        short ret = cnc_rdopnlsgnl(handle, 0x40, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.spdl_ovrd;
    }
    return res;
}

#pragma endregion

#pragma region Alarm data

ShortData GetEmergency(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBST buf;
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.emergency;
    }
    return res;
}

ShortData GetAlarm(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBST buf;
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.alarm;
    }
    return res;
}

#pragma endregion

#pragma region Operating data

LongData GetPowerOnTime(unsigned short& handle)
{
    LongData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBPSD buf;
        short ret = cnc_rdparam(handle, 6750, -1, sizeof(buf), &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.u.rdata.prm_val;
    }
    return res;
}

DoubleData GetOperationTime(unsigned short& handle)
{
    DoubleData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBPSD buf_1;
        short ret = cnc_rdparam(handle, 6751, -1, sizeof(buf_1), &buf_1);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            IODBPSD buf_2;
            ret = cnc_rdparam(handle, 6752, -1, sizeof(buf_2), &buf_2);
            if (ret != EW_OK)
                res.error = ret;
            else
                res.data = buf_2.u.rdata.prm_val * 60.0 + buf_1.u.rdata.prm_val / 1000.0;
        }
    }
    return res;
}

DoubleData GetCuttingTime(unsigned short& handle)
{
    DoubleData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBPSD buf_1;
        short ret = cnc_rdparam(handle, 6753, -1, sizeof(buf_1), &buf_1);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            IODBPSD buf_2;
            ret = cnc_rdparam(handle, 6754, -1, sizeof(buf_2), &buf_2);
            if (ret != EW_OK)
                res.error = ret;
            else
                res.data = buf_2.u.rdata.prm_val * 60.0 + buf_1.u.rdata.prm_val / 1000.0;
                //res.data = buf_2.u.rdata.prm_val + buf_1.u.rdata.prm_val / 60000.0;
        }
    }
    return res;
}

DoubleData GetCycleTime(unsigned short& handle)
{
    DoubleData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBPSD buf_1;
        short ret = cnc_rdparam(handle, 6757, -1, sizeof(buf_1), &buf_1);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            IODBPSD buf_2;
            ret = cnc_rdparam(handle, 6758, -1, sizeof(buf_2), &buf_2);
            if (ret != EW_OK)
                res.error = ret;
            else
                res.data = buf_2.u.rdata.prm_val * 60.0 + buf_1.u.rdata.prm_val / 1000.0;
                //res.data = buf_2.u.rdata.prm_val + buf_1.u.rdata.prm_val / 60000.0;
        }
    }
    return res;
}

StrData GetSeriesNumber(unsigned short& handle)
{
    StrData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSYS buf;
        short ret = cnc_sysinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.series;
    }
    return res;
}

StrData GetVersionNumber(unsigned short& handle)
{
    StrData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSYS buf;
        short ret = cnc_sysinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.version;
    }
    return res;
}

ShortData GetCtrlAxesNumber(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSYSEX  buf;
        short ret = cnc_sysinfo_ex(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.ctrl_axis;
    }
    return res;
}

ShortData GetCtrlSpindlesNumber(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSYSEX  buf;
        short ret = cnc_sysinfo_ex(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.ctrl_spdl;
    }
    return res;
}

ShortData GetCtrlPathsNumber(unsigned short& handle)
{
    ShortData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSYSEX  buf;
        short ret = cnc_sysinfo_ex(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.ctrl_path;
    }
    return res;
}

LongData GetSerialNumber(unsigned short& handle)
{
    LongData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBPSD buf;
        short ret = cnc_rdparam(handle, 13151, -1, sizeof(buf), &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.u.rdata.prm_val;
    }
    return res;
}

StrData GetCncId(unsigned short& handle)
{
    StrData res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        unsigned long  buf[4];
        short ret = cnc_rdcncid(handle, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = std::format("{:X}-{:X}-{:X}-{:X}", buf[0], buf[1], buf[2], buf[3]);
    }
    return res;
}
#pragma endregion