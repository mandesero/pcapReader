#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include "libs/header/RawPacket.h"
#include "libs/header/IPLayer.h"
#include "libs/header/Packet.h"
#include "output.h"
#include <vector>

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

int main() {
    int pipefd[2]; 
    if (pipe(pipefd) == -1) {
        std::cerr << "Failed to create pipe\n";
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork\n";
        return 1;

    } else if (pid == 0) {

        close(pipefd[1]);

        int pcap_format = -1;
        uint8_t buffer;

        uint8_t* h = new uint8_t[sizeof(PcapHdr)];
        int i = 0;
        while (i < sizeof(PcapHdr) && read(pipefd[0], &buffer, sizeof(buffer)) > 0) {
            h[i++] = buffer;
        }

        PcapHdr* pcap_header = (PcapHdr*)h;

        if (pcap_header->magic_number == 0xA1B2C3D4) {
            delete[] h;
            pcap_format = 1;
        }

        if (pcap_header->magic_number == 0xA1B23C4D) {
            delete[] h;
            pcap_format = 2;
        }

        if (pcap_format == -1) {
            delete[] h;
            std::cerr << "Error: file is not pcap." << std::endl;
            return 1;
        }

        while (true) {
            uint8_t* buff = new uint8_t[sizeof(PcapPacket)];

            int i = 0;

            while (i < sizeof(PcapPacket) && read(pipefd[0], &buffer, sizeof(buffer)) > 0) {
                buff[i++] = buffer;
                // std::cout << std::hex << static_cast<int>(buffer) << " ";
            }
            std::cout << "\n";
            if (buffer == EOF)
                break;
            
            if (i < sizeof(PcapPacket)) {
                delete[] buff;
                break;
            }

            PcapPacket* pcapPacketHdr = (PcapPacket*)buff;

            size_t len = pcapPacketHdr->included_length;

            timeval time{
                pcapPacketHdr->timestamp_seconds,
                pcapPacketHdr->timestamp_microseconds};

            uint8_t* packet = new uint8_t[len];

            i = 0;
            while (i < len && read(pipefd[0], &buffer, sizeof(buffer)) > 0) {
                packet[i++] = buffer;
                //  std::cout << static_cast<int>(buffer) << " ";
            }
            std::cout << std::endl;
            if (buffer == EOF)
                break;

            if (i < len) {
                delete[] buff;
                delete[] packet;
                break;
            }

            const uint8_t* const_pkt = packet;
            pcpp::RawPacket rawPacket(const_pkt, len, time, false);

            delete[] buff;
            delete[] packet;

            pcpp::Packet parsedPacket(&rawPacket);

            if (parsedPacket.isPacketOfType(pcpp::IPv4)) {
                pcpp::IPLayer* ipLayer = parsedPacket.getLayerOfType<pcpp::IPLayer>();

                std::string srcIP = ipLayer->getSrcIPAddress().toString();
                std::string dstIP = ipLayer->getDstIPAddress().toString();

                std::cout << srcIP << " " << dstIP << '\n';
            }
        }

        std::cout << std::endl;
        close(pipefd[0]);

        return 0;


    } else {


        close(pipefd[0]);

        uint8_t input;
        while (std::cin >> input) {
            write(pipefd[1], &input, sizeof(input));
        }
        input = EOF;
        write(pipefd[1], &input, sizeof(input));
        close(pipefd[1]);

        return 0;
    }
}
