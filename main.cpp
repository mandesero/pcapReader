#include "PcapReader.h"
#include "libs/header/IPLayer.h"
#include "libs/header/Packet.h"

int main(int argc, char* argv[]) {
    if (isPcapStream() == -1)
        return 1;

    pcpp::RawPacket rawPacket;

    while (getNextPacketFromSteam(rawPacket)) {
        pcpp::Packet parsedPacket(&rawPacket);
		if (parsedPacket.isPacketOfType(pcpp::IPv4)) {
			pcpp::IPLayer* ipLayer = parsedPacket.getLayerOfType<pcpp::IPLayer>();

			pcpp::IPAddress srcIP = ipLayer->getSrcIPAddress();
			pcpp::IPAddress dstIP = ipLayer->getDstIPAddress();

			std::cout << srcIP << " " << dstIP << std::endl;
		} else {
			std::cout << "Not IP packet" << std::endl;
		}
    }
}
