#include "PcapReader.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

int isPcapStream() {
    uint8_t* h = new uint8_t[sizeof(PcapHdr)];
    char bt;
    int i = 0;
    while (i < sizeof(PcapHdr) && (bt = fgetc(stdin)) != EOF) {
        h[i++] = bt;
    }
    PcapHdr* pcap_header = (PcapHdr*)h;

    if (pcap_header->magic_number == 0xA1B2C3D4) {
        delete[] h;
        return 1;
    }

    if (pcap_header->magic_number == 0xA1B23C4D) {
        delete[] h;
        return 2;
    }

    delete[] h;
    std::cerr << "Error: file is not pcap." << std::endl;
    return -1;
}

bool getNextPacketFromSteam(pcpp::RawPacket& rawPacket) {
    uint8_t* buff = new uint8_t[sizeof(PcapPacket)];
    char bt;
    int i = 0;

    while (i < sizeof(PcapPacket) && (bt = fgetc(stdin)) != EOF) {
        buff[i++] = bt;
    }
    std::cout << std::endl;
    
    
    if (i < sizeof(PcapPacket)) {
        delete[] buff;
        return false;
    }

    PcapPacket* pcapPacketHdr = (PcapPacket*)buff;

    size_t len = pcapPacketHdr->included_length;

    timeval time{
        pcapPacketHdr->timestamp_seconds,
        pcapPacketHdr->timestamp_microseconds};

    uint8_t* packet = new uint8_t[len];

    i = 0;
    while (i < len && (bt = fgetc(stdin)) != EOF) {
        packet[i++] = bt;
    }

    if (i < len) {
        delete[] buff;
        delete[] packet;
        return false;
    }

    const uint8_t* const_pkt = packet;
    rawPacket = pcpp::RawPacket(const_pkt, len, time, false);

    delete[] buff;
    delete[] packet;
    return true;
}