#pragma warning(disable: 4996)
#pragma comment(lib, "focas/fwlib32.lib")
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "focas/fwlib32.h"
#include <string>
#include <iostream>
#include <cmath>
#include "FanucTypes.h"
#define _CRT_SECURE_NO_WARNINGS

#pragma region Handle functions

ushort_data GetHandle(std::string address, int port, int timeout)
{
    ushort_data res = {};
    short ret = cnc_allclibhndl3(address.c_str(), port, timeout, &res.data);
    if (ret != EW_OK)
        res.error = ret;
    return res;
}

void_func FreeHandle(unsigned short handle)
{
    void_func res = {};
    short ret = cnc_freelibhndl(handle);
    if (ret != EW_OK)
        res.error = ret;
    return res;
}

#pragma endregion

#pragma region Mode data

short_data GetMode(unsigned short handle)
{
    short_data res = {};
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

short_data GetRunState(unsigned short handle)
{
    short_data res = {};
    if (handle == 0)
    {
       res.error = -8;
    }
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

short_data GetStatus(unsigned short handle)
{
    short_data res = {};
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

short_data GetShutdowns(unsigned short handle)
{
    short_data res = {};
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

short_data GetHightSpeed(unsigned short handle)
{
    short_data res = {};
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
            if (strstr(buf, "G00"))
                res.data = 0;
            else
                res.data = 1;
        }
    }
    return res;
}

short_data GetAxisMotion(unsigned short handle)
{
    short_data res = {};
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

short_data GetMstb(unsigned short handle)
{
    short_data res = {};
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

//test
long_data GetLoadExcess(unsigned short handle)
{
    long_data res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        long alarm;
        short ret = cnc_alarm2(handle, &alarm);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = alarm;
    }
    return res;
}

#pragma endregion

#pragma region Program data

short_data GetMainPrgNumber(unsigned short handle)
{
    short_data res = {};
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

short_data GetSubPrgNumber(unsigned short handle)
{
    short_data res = {};
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

str_data GetFrame(unsigned short handle)
{
    str_data res = {};
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
        else if (length != 0 && blknum >= 0 && blknum + length <= sizeof(buf))
            res.data = std::string(buf + blknum, length);
    }
    return res;
}

int_data GetPartsCount(unsigned short handle)
{
    int_data res = {};
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

//test
long_data GetToolNumber(unsigned short handle)
{
    long_data res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBTLMNG buf = {};
        short data_num = 1;
        short ret = cnc_rdtool(handle, 1, &data_num, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.T_code;
    }
    return res;
}

long_data GetFrameNumber(unsigned short handle)
{
    long_data res = {};
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

#pragma endregion

#pragma region Axis data

long_data GetFeedRate(unsigned short handle)
{
    long_data res = {};
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

short_data GetFeedOverride(unsigned short handle)
{
    short_data res = {};
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

//test
//short result = Focas1.cnc_rdparam(handle, 1401, 0, 8, jov);
//bool jog_enabled = (jov.cdata & 0x01) != 0;
short_data GetJogOverride(unsigned short handle)
{
    short_data res = {};
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

//1423 parametrer (jog feed)
long_data GetJogSpeed(unsigned short handle)
{
    long_data res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBAXDT buf[MAX_AXIS] = {};
        short lenght = MAX_AXIS;
        short type = 5;
        short ret = cnc_rdaxisdata(handle, 5, &type, 5, &lenght, buf);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            bool jog_enabled = false;
            for (int i = type * MAX_AXIS; i < type * MAX_AXIS + lenght; i++) 
            {
                jog_enabled = (buf[i].flag & 0x02) != 0;
                if (jog_enabled)
                {
                    res.data = buf[i].data;
                    return res;
                }
            }
            //res.error_msg = "Jog отключен";
        }
    }
    return res;
}

map_data GetAllServoLoad(unsigned short handle)
{
    map_data res = {};
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

// N : 2086 Word axis Rated current parameter
float_data GetServoCurrentLoad(unsigned short handle)
{
    float_data res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBPSD n;
        short ret = cnc_rdparam(handle, 2086, -1, sizeof(n), &n);
        if (ret != EW_OK)
            res.error = ret;
        else
        {
            ODBAD parameter = {};
            short ret = cnc_adcnv(handle, 2, 2, &parameter);
            if (ret != EW_OK)
            {
                res.error = ret;
                return res;
            }
            else
            {
                long prm = n.u.rdata.prm_val;
                long dec = n.u.rdata.dec_val;

                if (dec > 0)
                {
                    float value = prm * std::pow(10, -dec);
                    res.data = parameter.data * value / 6554;
                }
            }
        }

    }
    return res;
}

// N : 2086 Word axis Rated current parameter
// Max : 2165 Word axis Maximum amplifier current
float_data GetServoCurrentPercentLoad(unsigned short handle)
{
    float_data res = {};
    if (handle == 0)
        res.error = -8;
    float_data current_data = GetServoCurrentLoad(handle);
    if (current_data.IsError())
        res.error = current_data.error;
    else
    {
        IODBPSD max = {};
        short ret = cnc_rdparam(handle, 2165, -1, sizeof(max), &max);
        if (ret != EW_OK)
        {
            res.error = ret;
            return res;
        }
        else
        {
            long prm = max.u.rdata.prm_val;
            long dec = max.u.rdata.dec_val;
            if (dec > 0)
            {
                float value = prm * std::pow(10, -dec);
                res.data = (current_data.data / value) * 100;
            }
        }
    }
    return res;
}
#pragma endregion

#pragma region Spindle data

long_data GetSpindleSpeed(unsigned short handle)
{
    long_data res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBSPEED buf = {};
        short type = 1;
        short ret = cnc_rdspeed(handle, type, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.acts.data;
    }
    return res;
}

long_data GetSpindleSpeedParam(unsigned short handle)
{
    long_data res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBPSD buf;
        short ret = cnc_rdparam(handle, 3799, -1, sizeof(buf), &buf);

        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.u.ldata;
    }
    return res;
}

map_data GetSpindleMotorSpeed(unsigned short handle)
{
    map_data res = {};
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
            for (int i = 0; i < num; i++)
            {
                std::string name = std::string(1, buf[i].spspeed.name);
                int value = buf[i].spspeed.data;
                res.data[name] = value;
            }
        }
    }
    return res;
}

map_data GetSpindleLoad(unsigned short handle)
{
    map_data res = {};
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
            for (int i = 0; i < num; i++)
            {
                std::string name = std::string(1, buf[i].spload.name);
                int value = buf[i].spload.data;
                res.data[name] = value;
            }
        }
    }
    return res;
}

//test
short_data GetSpindleOverride(unsigned short handle)
{
    short_data res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        IODBSGNL buf = {};
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

short_data GetEmergencyStop(unsigned short handle)
{
    short_data res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBST buf = {};
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
            res.error = ret;
        else
            res.data = buf.emergency;
    }
    return res;
}

short_data GetAlarmStatus(unsigned short handle)
{
    short_data res = {};
    if (handle == 0)
        res.error = -8;
    else
    {
        ODBST buf = {};
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



#pragma endregion