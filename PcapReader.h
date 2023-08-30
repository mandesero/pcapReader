#pragma once
#include <fstream>
#include <iomanip>
#include <iostream>

#include "libs/header/RawPacket.h"

struct PcapHdr {
    uint32_t magic_number;   // Магическое число (идентификатор формата файла)
    uint16_t version_major;  // Основная версия
    uint16_t version_minor;  // Второстепенная версия
    uint32_t thiszone;       // Зона времени
    uint32_t sigfigs;        // Точность временных меток
    uint32_t snaplen;        // Максимальный размер пакета
    uint32_t network;        // Тип сети
};

struct PcapPacket {
    uint32_t timestamp_seconds;       // Время прихода пакета в секундах
    uint32_t timestamp_microseconds;  // Время прихода пакета в микросекундах
    uint32_t included_length;         // Длина пакета, сохраненная в файле
    uint32_t original_length;         // Оригинальная длина пакета
};


int isPcapStream(std::ifstream& input);
bool getNextPacketFromSteam(pcpp::RawPacket& rawPacket, std::ifstream& input);