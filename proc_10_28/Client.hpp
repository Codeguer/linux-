#ifndef __Client_HPP__
#define __Client_HPP__

#include<iostream>
#include<string>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include"Protocol.hpp"

using namespace std;
class client{
  private:
    int port;
    string ip;
    int sock;
  public:
    client(string _ip,int _port):ip(_ip),port(_port),sock(-1)
    {}
    void initClient(){
      sock=socket(AF_INET,SOCK_STREAM,0);
      if(sock < 0){
        cerr<<"socket error "<<endl;
        exit(2);
      }
    }

    void start(){
      struct sockaddr_in server;
      server.sin_family=AF_INET;
      server.sin_port=htons(port);
      server.sin_addr.s_addr=inet_addr(ip.c_str());

      if(connect(sock,(struct sockaddr*)&server,sizeof(server)) < 0){
          cerr<<"connect error!" <<endl;
          exit(2);
      }
      
      response_t rsp;
      request_t rq;
      cout<<"data1# ";
      cin>>rq.x;
      cout<<"data2# ";
      cin>>rq.y;
      cout<<"op# ";
      cin>>rq.op;

      send(sock,&rq,sizeof(rq),0);
      recv(sock,&rsp,sizeof(rsp),0);

      cout<<"code: "<<rsp.code<<endl;
      cout<<"result"<<rsp.result<<endl;
    }

    ~client(){
      close(sock);
    }
};


#endif
