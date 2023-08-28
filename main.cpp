#include "PcapReader.h"
#include "libs/header/IPLayer.h"
#include "libs/header/Packet.h"
#include "output.h"
#include <vector>

int main(int argc, char* argv[]) {
    if (isPcapStream() == -1)
        return 1;

    pcpp::RawPacket rawPacket;

    int count = 0;
    while (getNextPacketFromSteam(rawPacket)) {
        
        pcpp::Packet parsedPacket(&rawPacket);
        if (parsedPacket.isPacketOfType(pcpp::IPv4)) {
            pcpp::IPLayer* ipLayer = parsedPacket.getLayerOfType<pcpp::IPLayer>();

            std::string srcIP = ipLayer->getSrcIPAddress().toString();
            std::string dstIP = ipLayer->getDstIPAddress().toString();

            std::cout << srcIP << " " << dstIP << '\n';
        } else {
            std::cout << "Not IP packet" << '\n';
        }
        count++;
    }
    std::cout << "Packets: " << count << std::endl;
    return 0;
}
