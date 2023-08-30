#include "PcapReader.h"
#include "libs/header/IPLayer.h"
#include "libs/header/Packet.h"
#include "output.h"
#include <cstdio>
#include <vector>
#include <fstream>

int main(int argc, char* argv[]) {
    std::ifstream input;
    input.open("/dev/stdin", std::ios::binary);
    if (!input) {
        std::cout << "stdin open error" << std::endl;
        return 1;
    }

    if (isPcapStream(input) == -1)
        return 1;

    pcpp::RawPacket rawPacket;

    int count = 0;
    while (getNextPacketFromSteam(rawPacket, input)) {
        
        pcpp::Packet parsedPacket(&rawPacket);
        if (parsedPacket.isPacketOfType(pcpp::IPv4)) {
            pcpp::IPLayer* ipLayer = parsedPacket.getLayerOfType<pcpp::IPLayer>();

            std::string srcIP = ipLayer->getSrcIPAddress().toString();
            std::string dstIP = ipLayer->getDstIPAddress().toString();

            std::cout << srcIP << " " << dstIP << '\n';
        } else {
            std::cout << "Not IPv4 packet" << '\n';
        }
        count++;
    }
    std::cout << "Packets: " << count << std::endl;
    input.close();
    
    return 0;
}
