
FLAG=-std=c++11 -lpthread

.PHONY:all
all:Client Server

Client:Client.cc
	g++ -o $@ $^ $(FLAG) -static
Server:Server.cc 
	g++ -o $@ $^ $(FLAG) 

.PHONY:clean
clean:
	rm -f Client Server
