#include "PcapReader.h"

int isPcapStream() {
    uint8_t* h = new uint8_t[sizeof(PcapHdr)];
    uint8_t bt;
    int i = 0;
    while (i < sizeof(PcapHdr) && std::cin >> bt) {
        h[i++] = bt;
    }
    PcapHdr* pcap_header = (PcapHdr*)h;

    if (pcap_header->magic_number == 0xA1B2C3D4) {
        delete[] h;
        // std::cout << "0xA1B2C3D4\n";
        return 1;
    }

    if (pcap_header->magic_number == 0xA1B23C4D) {
        delete[] h;
        // std::cout << "0xA1B23C4D\n";
        return 2;
    }

    delete[] h;
    std::cerr << "Error: file is not pcap." << std::endl;
    return -1;
}

bool getNextPacketFromSteam(pcpp::RawPacket& rawPacket) {
    uint8_t* buff = new uint8_t[sizeof(PcapPacket)];
    uint8_t bt;

    int i = 0;

    while (i < sizeof(PcapPacket) && std::cin >> bt) {
        buff[i++] = bt;
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bt) << ' ';
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
    while (i < len && std::cin >> bt) {
        packet[i++] = bt;
    }

    if (i < len) {
        delete[] buff;
        delete[] packet;
        return false;
    }

    const uint8_t* const_pkt = packet;
    rawPacket = pcpp::RawPacket(const_pkt, len, time, false);

    for (int i = 0; i < len; i++) {
        if (i % 16 == 0 && i != 0)
            std::cout << '\n';
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet[i]) << ' ';
    }
    std::cout << '\n';
    delete[] buff;
    delete[] packet;
    return true;
}