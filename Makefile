include libs/mk/PcapPlusPlus.mk

#fork
# all:	
# 	g++ $(PCAPPP_INCLUDES) -c fork.cpp
# 	g++ $(PCAPPP_LIBS_DIR) -o main fork.o $(PCAPPP_LIBS)
# 	rm *.o

#buff
all:	
	g++ $(PCAPPP_INCLUDES) -c main.cpp
	g++ $(PCAPPP_INCLUDES) -c PcapReader.cpp
	g++ $(PCAPPP_LIBS_DIR) -o main main.o PcapReader.o $(PCAPPP_LIBS)
	rm *.o


clean:
	rm *.txt
	rm *.pcap
	rm main