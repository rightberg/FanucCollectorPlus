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

#pragma region Sub functions

std::string GetCncErrorMessage(int error_code) {
    switch (error_code) 
    {
        case -17: return "Protocol error";
        case -16: return "Socket error";
        case -15: return "DLL file error";
        case -11: return "Bus error";
        case -10: return "System error (2)";
        case -9: return "Communication error of HSSB";
        case -8: return "Handle number error";
        case -7: return "Version mismatch between the CNC/PMC and library";
        case -6: return "Abnormal library state";
        case -5: return "System error";
        case -4: return "Shared RAM parity error";
        case -3: return "FANUC drivers installation error";
        case -2: return "Reset or stop request";
        case -1: return "Busy";
        case 0: return "Normal termination";
        case 1: return "Error(function is not executed, or not available)";
        case 2: return "Error(data block length error, error of number of data)";
        case 3: return "Error(data number error)";
        case 4: return "Error(data attribute error)";
        case 5: return "Error(data error)";
        case 6: return "Error(no option)";
        case 7: return "Error(write protection)";
        case 8: return "Error(memory overflow)";
        case 9: return "Error(CNC parameter error)";
        case 10: return "Error(buffer empty/full)";
        case 11: return "Error(path number error)";
        case 12: return "Error(CNC mode error)";
        case 13: return "Error(CNC execution rejection)";
        case 14: return "Error(Data server error)";
        case 15: return "Error(alarm)";
        case 16: return "Error(stop)";
        case 17: return "Error(State of data protection)";
        default: return "Unknown error";
    }
}

std::string GetModeString(short mode, std::string series) 
{
    if (series == "0i-D" || series == "0i-F") 
    {
        switch (mode) 
        {
            case 0: return "Полуавтомат (MDI)";
            case 1: return "Автоматический (MEM)";
            case 3: return "Редактирование (EDIT)";
            case 4: return "Ручная подача с помощью маховика (HND)";
            case 5: return "Ручная непрерывная подача (JOG)";
            case 6: return "Обучение JOG (Tech in JOG)";
            case 7: return "Обучение HND (Teach In Handle)";
            case 8: return "Ручные фиксированные перемещения (INC)";
            case 9: return "Выход в ноль (REF)";
            case 10: return "Автоматический DNC (RMT)";
            default: return "Другой режим (Other)";
        }
    }
    return "Серия контроллера не поддерживается";
}

std::string GetRunStateString(int state_num, std::string series)
{
    if (series == "0i-D" || series == "0i-F")
    {
        switch (state_num)
        {
            case 0: return "Сброс (RES)";
            case 1: return "Останов автоматической работы (STOP)";
            case 2: return "Приостанов автоматической работы (HOLD)";
            case 3: return "Выполнение УП (STRT)";
            case 4: return "Отвод и возврат инструмента (MSTR)";
            default: return "Неизвестное состояние";
        }
    }
    return "Серия контроллера не поддерживается";
}

std::string GetStatusString(int status) 
{
    switch (status) 
    {
        case 0: return "**** (Отсутствие редактирования УП)";
        case 1: return "EDIT (Редактирование УП)";
        case 2: return "SeaRCH (Поиск данных)";
        case 3: return "OUTPUT (Вывод данных)";
        case 4: return "INPUT (Ввод данных)";
        case 5: return "COMPARE (Сравнение данных)";
        case 6: return "Label Skip (Пропуск метки)";
        case 7: return "ReSTaRt (Перезапуск программы)";
        case 8: return "Work ShiFT (Режим измерения величины смещения начала системы координат детали)";
        case 9: return "PTRR (Отвод или повторное позиционирование инструмента)";
        case 10: return "RVRS (Реверсирование)";
        case 11: return "RTRY (Повторный подвод)";
        case 12: return "RVED (Завершение реверсирования)";
        case 13: return "HANDLE (Перекрытие обработчика)";
        case 14: return "OFfSeT (Режим измерения величины коррекции инструмента по длине)";
        case 15: return "Work OFfSet (Режим измерения нулевой точки работы)";
        case 16: return "AICC (Режим контурного управления с СИИ)";
        case 17: return "MEmory-CHecK (Проверка памяти программ)";
        case 18: return "CusToMer's BoarD (Контроль платы заказчика)";
        case 19: return "SAVE (Сохранение данных измерения крутящего момента)";
        case 20: return "AI NANO (Нанопрограммное контурное управление с СИИ)";
        case 21: return "AI APC (Режим контурного управления с прогнозированием и СИИ)";
        case 22: return "Многоблочный расширенный контроль предварительного просмотра";
        case 23: return "AICC 2 (Высокоточное контурное управление с СИИ)";
        case 24: return "Высокоточное нанопрограммное контурное управление с СИИ";
        case 26: return "OFSX (Режим изменения активной величины коррекции инструмента по оси Х)";
        case 27: return "OFSZ (Режим изменения активной величины коррекции инструмента по оси Z)";
        case 28: return "WZR (Режим изменения смещения начала координат заготовки)";
        case 29: return "OFSY (Режим изменения величины активной коррекции инструмента по оси Y)";
        case 30: return "LEN (Режим изменения смещения длины/оси Х)";
        case 31: return "TOFS (Режим изменения коррекции инструмента)";
        case 32: return "RAD (Режим изменения величины коррекции на радиус инструмента)";
        case 39: return "TCP (Режим управления центром инструмента при обработке по 5 осям)";
        case 40: return "TWP (Режим наклонной рабочей плоскости)";
        case 41: return "TCP+TWP (Управление центром инструмента при 5-осевой обработке и наклонной плоскости обработки)";
        case 42: return "APC (Расширенный режим управления предварительным просмотром)";
        case 43: return "PRG-CHK (Проверка программы на высокой скорости)";
        case 44: return "APC (Расширенный режим управления предварительным просмотром)";
        case 45: return "S-TCP (Плавное управление центральной точкой инструмента)";
        case 46: return "AICC 2 (Контурное управление с СИИ 2)";
        case 59: return "ALLSAVE (Высокоскоростное управление программами: выполняется сохранение программ)";
        case 60: return "NOTSAVE (Высокоскоростное управление программами: статус 'программы не сохраняются')";
        default: return "Неизвестный статус";
    }
}

std::string GetAxisMotionString(int motion, const std::string series) 
{
    if (series == "0i-D" || series == "0i-F") {
        switch (motion) 
        {
            case 1: return "Останов автоматической работы (MTN)";
            case 2: return "Останов автоматической работы (DWL)";
            default: return "Другой режим (Others)";
        }
    }
    return "Серия контроллера не поддерживается";
}

std::string GetMstbString(int mstb, const std::string series) 
{
    if (series == "0i-D" || series == "0i-F") 
    {
        switch (mstb) 
        {
            case 2: return "Выполнение M,S,T функции (FIN)";
            default: return "Другой режим (Others)";
        }
    }
    return "Серия контроллера не поддерживается";
}

std::string GetLoadExcessString(int mstb, const std::string series)
{
    if (series == "0i-D" || series == "0i-F")
    {
        switch (mstb)
        {
            case 6: return "Превышение нагрузки на оси (AXES LOAD EXCESS)";
            case 9: return "Превышение нагрузки на шпиндель (SPINDLE LOAD EXCESS)";
            default: return "Отсутствие превышения (NO EXCESS)";
        }
    }
    return "Серия контроллера не поддерживается";
}

std::string GetUnitString(int unit)
{
    switch (unit)
    {
        case 0: return "mm/min";
        case 1: return "inch/min";
        case 2: return "rpm";
        case 3: return "mm/rev";
        case 4: return "inch/rev";
        default: return "Нет данных по ед. изм.";
    }
}

int GetFeedOverrideValue(short unit)
{
    switch (unit)
    {
        case 0: return 0;
        case 1: return 10;
        case 2: return 20;
        case 3: return 30;
        case 4: return 40;
        case 5: return 50;
        case 6: return 60;
        case 7: return 70;
        case 8: return 80;
        case 9: return 90;
        case 10: return 100;
        case 11: return 110;
        case 12: return 120;
        case 13: return 130;
        case 14: return 140;
        case 15: return 150;
        case 16: return 160;
        case 17: return 170;
        case 18: return 180;
        case 19: return 190;
        case 20: return 200;
        default: return 0;
    }
}

float GetJogOverrideValue(short jog)
{
    switch (jog)
    {
        case 0: return 0;
        case 1: return 0.1f;
        case 2: return 0.14f;
        case 3: return 0.2f;
        case 4: return 0.27f;
        case 5: return 0.37f;
        case 6: return 0.52f;
        case 7: return 0.72f;
        case 8: return 1.0f;
        case 9: return 1.4f;
        case 10: return 2.0f;
        case 11: return 2.7f;
        case 12: return 3.7f;
        case 13: return 5.2f;
        case 14: return 7.2f;
        case 15: return 10.0f;
        case 16: return 14.0f;
        case 17: return 20.0f;
        case 18: return 27.0f;
        case 19: return 37.0f;
        case 20: return 52.0f;
        case 21: return 72.0f;
        case 22: return 100.0f;
        case 23: return 140.0f;
        case 24: return 200.0f;
        default: return 0;
    }
}

std::string FindShutdowns(const char* data)
{
    const char* commands[] = { "M00", "M01", "G04" };
    const int commands_count = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < commands_count; i++)
    {
        if (strstr(data, commands[i]))
            return commands[i];
    }
    return "No stop";
}

#pragma endregion

#pragma region Mode data

ushort_data GetHandle(std::string address, int port, int timeout)
{
    ushort_data res = {};
    short ret = cnc_allclibhndl3(address.c_str(), port, timeout, &res.data);
    if (ret != EW_OK)
    {
        res.error = true;
        res.error_msg = GetCncErrorMessage(ret);
    }
    return res;
}

void_func FreeHandle(unsigned short handle)
{
    void_func res = {};
    short ret = cnc_freelibhndl(handle);
    if (ret != EW_OK)
    {
        res.error = true;
        res.error_msg = GetCncErrorMessage(ret);
    }
    return res;
}

str_data GetMainPrgName(unsigned short handle)
{
    str_data res = {};
	if (handle == 0) 
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
	}
	else 
    {
		ODBEXEPRG buf = {};
		short ret = cnc_exeprgname(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = buf.name;
	}
    return res;
}

str_data GetMode(unsigned short handle, std::string series)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBST buf = {};
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetModeString(buf.aut, series);
    }
    return res;
}

str_data GetRunState(unsigned short handle, std::string series)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBST buf = {};
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetRunStateString(buf.run, series);
    }
    return res;
}

str_data GetStatus(unsigned short handle, std::string series)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBST buf = {};
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetStatusString(buf.edit);
    }
    return res;
}

str_data GetShutdowns(unsigned short handle)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        unsigned short length = 256;
        short blknum;
        char buf[256];
        short ret = cnc_rdexecprog(handle, &length, &blknum, buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = FindShutdowns(buf);
    }
    return res;
}

str_data GetHightSpeed(unsigned short handle)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        unsigned short length = 256;
        short blknum;
        char buf[256];
        short ret = cnc_rdexecprog(handle, &length, &blknum, buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
        {
            if (strstr(buf, "G00"))
                res.data = "G00 активен";
            else
                res.data = "G00 не активен";
        }
    }
    return res;
}

str_data GetAxisMotion(unsigned short handle, std::string series)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBST buf = {};
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetAxisMotionString(buf.motion, series);
    }
    return res;
}

str_data GetMstb(unsigned short handle, std::string series)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBST buf = {};
        short ret = cnc_statinfo(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetMstbString(buf.mstb, series);
    }
    return res;
}

str_data GetLoadExcess(unsigned short handle, std::string series)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        long alarm;
        short ret = cnc_alarm2(handle, &alarm);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetLoadExcessString(alarm, series);
    }
    return res;
}

#pragma endregion

#pragma region Program data

int_data GetMainPrgNumber(unsigned short handle)
{
    int_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBPRO buf = {};
        short ret = cnc_rdprgnum(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = buf.mdata;
    }
    return res;
}

int_data GetSubPrgNumber(unsigned short handle)
{
    int_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBPRO buf = {};
        short ret = cnc_rdprgnum(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = buf.data;
    }
    return res;
}

str_data GetFrame(unsigned short handle)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        unsigned short length = 256;
        short blknum;
        char buf[256];
        short ret = cnc_rdexecprog(handle, &length, &blknum, buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
        {
            if (length == 0 || blknum < 0 || blknum + length > sizeof(buf)) 
            {
                res.error = true;
                res.error_msg = "Некорректные данные блока";
            }
            else
                res.data = std::string(buf + blknum, length);
        }

    }
    return res;
}


int_data GetPartsCount(unsigned short handle)
{
    int_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        IODBPSD buf = {};
        short ret = cnc_rdparam(handle, 6771, 0, 8, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
            return res;
        }
        else
        {
            long prm = buf.u.rdata.prm_val;
            long dec = buf.u.rdata.dec_val;

            if (dec < 0 || dec > 10) 
            {
                res.error = true;
                res.error_msg = "Некорректное значение dec_val";
                return res;
            }

            res.data = prm * std::pow(10, -dec);
        }
    }
    return res;
}

int_data GetToolNumber(unsigned short handle)
{
    int_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        IODBTLMNG buf = {};
        short data_num = 1;
        short ret = cnc_rdtool(handle, 1, &data_num, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = buf.T_code;
    }
    return res;
}

int_data GetFrameNum(unsigned short handle)
{
    int_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBSEQ buf = {};
        short ret = cnc_rdseqnum(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = buf.data;
    }
    return res;
}

#pragma endregion


#pragma region Axis data

str_data GetFeedUnit(unsigned short handle)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBSPEED buf = {};
        short type = 0;
        short ret = cnc_rdspeed(handle, type, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetUnitString(buf.actf.unit);
    }
    return res;
}

str_data GetSpindleUnit(unsigned short handle)
{
    str_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBSPEED buf = {};
        short type = 1;
        short ret = cnc_rdspeed(handle, type, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetUnitString(buf.actf.unit);
    }
    return res;
}

int_data GetFeedRate(unsigned short handle)
{
    int_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBACT buf = {};
        short ret = cnc_actf(handle, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = buf.data;
    }
    return res;
}

int_data GetFeedOverride(unsigned short handle)
{
    int_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        IODBSGNL buf = {};
        short ret = cnc_rdopnlsgnl(handle, 0x20, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetFeedOverrideValue(buf.feed_ovrd);
    }
    return res;
}

//short result = Focas1.cnc_rdparam(handle, 1401, 0, 8, jov);
//bool jog_enabled = (jov.cdata & 0x01) != 0;
float_data GetJogOverride(unsigned short handle)
{
    float_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        IODBSGNL buf = {};
        short ret = cnc_rdopnlsgnl(handle, 0x20, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
            res.data = GetJogOverrideValue(buf.jog_ovrd);
    }
    return res;
}

//1423 parametrer (jog feed)
int_data GetJogSpeed(unsigned short handle)
{
    int_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        ODBAXDT buf = {};
        short lenght = MAX_AXIS;
        short type = 5;
        short ret = cnc_rdaxisdata(handle, 5, &type, 5, &lenght, &buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
        }
        else
        {
            bool jog_enabled = (buf.flag & 0x02) != 0;
            if (!jog_enabled)
            {
                res.error = true;
                res.error_msg = "Jog отключен";
            }
            else
                res.data = buf.data;
        }
    }
    return res;
}

map_data GetAllServoLoad(unsigned short handle)
{
    map_data res = {};
    if (handle == 0)
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        short num = MAX_AXIS;
        ODBSVLOAD buf[MAX_AXIS] = {};
        short ret = cnc_rdsvmeter(handle, &num, buf);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
            return res;
        }
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
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    else
    {
        IODBPSD n = {};
        short ret = cnc_rdparam(handle, 2086, 0, 8, &n);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
            return res;
        }
        else
        {
            ODBAD parameter = {};
            short ret = cnc_adcnv(handle, 2, 2, &parameter);
            if (ret != EW_OK)
            {
                res.error = true;
                res.error_msg = GetCncErrorMessage(ret);
                return res;
            }
            else
            {
                long prm = n.u.rdata.prm_val;
                long dec = n.u.rdata.dec_val;

                if (dec < 0 || dec > 10)
                {
                    res.error = true;
                    res.error_msg = "Некорректное значение dec_val";
                    return res;
                }

                float value = prm * std::pow(10, -dec);

                res.data = parameter.data * value / 6554;
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
    {
        res.error = true;
        res.error_msg = "НЕТ ДОСТУПА";
    }
    float_data current_data = GetServoCurrentLoad(handle);
    if (current_data.error)
    {
        res.error = true;
        res.error_msg = current_data.error_msg;
        return res;
    }
    else
    {
        IODBPSD max = {};
        short ret = cnc_rdparam(handle, 2165, 0, 8, &max);
        if (ret != EW_OK)
        {
            res.error = true;
            res.error_msg = GetCncErrorMessage(ret);
            return res;
        }
        else
        {
            long prm = max.u.rdata.prm_val;
            long dec = max.u.rdata.dec_val;

            if (dec < 0 || dec > 10)
            {
                res.error = true;
                res.error_msg = "Некорректное значение dec_val";
                return res;
            }

            float value = prm * std::pow(10, -dec);

            res.data = (current_data.data / value) * 100;
        }
    }
    return res;
}
#pragma endregion