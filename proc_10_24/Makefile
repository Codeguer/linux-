
FLAG=-std=c++11

.PHONY:all
all:udpClient udpServer

udpClient:udpClient.cc
	g++ -o $@ $^ $(FLAG) -static 
udpServer:udpServer.cc
	g++ -o $@ $^ $(FLAG)

.PHONY:clean
clean:
	rm -f udpClient udpServer
