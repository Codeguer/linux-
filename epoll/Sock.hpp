#pragma once

#include<iostream>
#include<string>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<strings.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/epoll.h>
#include<cstring>

using namespace std;

class Sock{
  public:
    static int Socket(){
      int sock=socket(AF_INET,SOCK_STREAM,0);
      if(sock < 0){
        cerr<<"socket error"<<endl;
        exit(2);
      }
      return sock;
    }
    static void Bind(int sock,int port){
      struct sockaddr_in local;
      bzero(&local,sizeof(local));//清空内存空间
      local.sin_family=AF_INET;
      local.sin_port=htons(port);
      local.sin_addr.s_addr=htonl(INADDR_ANY);

      if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
        cerr<<"bind error"<<endl;
        exit(3);
      }
    }
    static void Listen(int sock){
      if(listen(sock,5) < 0){
        cerr<<"listen error"<<endl;
        exit(4);
      }
    }

    static int Accept(int sock){
     struct sockaddr_in peer; 
     socklen_t len=sizeof(peer);
     int fd=accept(sock,(struct sockaddr*)&peer,&len);
     if(fd < 0){
       cerr<<"accept error"<<endl;
     }

     return fd;
    }

    static void Setsockopt(int sock){
      //将端口设为复用状态，防止主动断开链接的一方进入time_wait的时候
      //进而导致服务器无法重启的问题
      int opt=1;
      setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    }
};
