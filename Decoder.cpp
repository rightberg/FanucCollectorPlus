#include <string>

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

std::string GetHightSpeedString(int index)
{
    switch (index)
    {
        case 0: return "G00 активен";
        case 1: return "G00 неактивен";
        default: return "Невереные данные";
    }
}

std::string GetAlarmString(int alarm, const std::string series)
{
    if (series == "0i-D" || series == "0i-F")
    {
        switch (alarm)
        {
            case 0: return "Включение параметрического переключателя (PARAMETER SWITCH ON)";
            case 1: return "Установка параметра при выключении питания (POWER OFF PARAMETER SET)";
            case 2: return "Ошибка ввода/вывода (I/O ERROR)";
            case 3: return "Предварительный P/S (FOREGROUND P/S)";
            case 4: return "Перемещение за пределы, внешние данные (OVERTRAVEL, EXTERNAL DATA)";
            case 5: return "Сигнал перегрева (OVERHEAT ALARM)";
            case 6: return "Сигнал ошибки сервопривода (SERVO ALARM)";
            case 7: return "Ошибка ввода/вывода данных (DATA I/O ERROR)";
            case 8: return "Сигнал ошибки макроса (MACRO ALARM)";
            case 9: return "Сигнал ошибки шпинделя (SPINDLE ALARM)";
            case 10: return "Другие сигналы ошибки (OTHER ALARM)";
            case 11: return "Ошибка функций предотвращения неисправностей (MALFUNCTION PREVENT FUNCTIONS)";
            case 12: return "Фоновый P/S (BACKGROUND P/S)";
            case 13: return "Ошибка синхронизации (SYNCHRONIZED ERROR)";
            case 15: return "Внешнее сообщение об ошибке (EXTERNAL ALARM MESSAGE)";
            default: return "Неизвестный сигнал (UNKNOWN SIGNAL)";
        }
    }
    return "Серия контроллера не поддерживается";
}

std::string GetAlarmStatusString(int alarm, const std::string series)
{
    if (series == "0i-D" || series == "0i-F")
    {
        switch (alarm)
        {
            case 0: return "****(Others))";
            case 1: return "Сигнал тревоги (ALARM)";
            case 2: return "Низкий уровень батареи (BATTERY LOW)";
            case 3: return "Предупреждение: Вентилятор (CNC, SA)(FAN)";
            case 4: return "Предупреждение: P/S (блок питания) (PS Warning)";
            case 5: return "Предупреждение FSSB (FSSB Warning)";
            case 6: return "Предупреждение утечки (INSULATE Warning)";
            case 7: return "Предупреждение датчика положения (Encoder Warning)";
            case 8: return "Низкий уровень батареи (PMC alarm)";
            default: return "Неизвестный сигнал тревоги";
        }
    }
    return "Серия контроллера не поддерживается";
}

std::string GetEmergencyString(int emg, const std::string series)
{
    if (series == "0i-D" || series == "0i-F")
    {
        switch (emg)
        {
            case 0: return "****(Not emergency)";
            case 1: return "Аварийная ситуация (Emergency)";
            case 2: return "Сброс (Reset)";
            case 3: return "Ожидание (Wait, FS35i only)";
            default: return "Неизвестное состояние";
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

std::string GetShutdownsString(short index)
{
    switch (index)
    {
    case 0: return "M00";
    case 1: return "M01";
    case 2: return "G04";
    case 3: return "No stop";
    default: return "Неверные данные";
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

int GetSpindleOverrideValue(short sp_ov)
{
    switch (sp_ov)
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