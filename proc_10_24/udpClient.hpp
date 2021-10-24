#pragma once

#include<iostream>
#include<string>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

class udpClient{
  private:
    std::string ip;
    int port;
    //这里的ip与port是在用户层创建出来的
    int sock;
  public:
    //要连服务器那么这里应该使用的是服务器的ip与端口号
    udpClient(std::string _ip="127.0.0.1",int _port=8080)//127.0.0.1为本地环回，使用本地环回可以进行代码测试
      //即用我们对应的client来连我的server,在一台机器上实现通信就可以
      :ip(_ip),port(_port)
    {
    }
    void initClient(){
      sock=socket(AF_INET,SOCK_DGRAM,0) ;//套接字是在内核层创建出来的
      std:: cout<<"sock: "<<sock<<std::endl;//sock默认是3
      //客户端不需要绑定
    }

    void start(){
      std::string msg;
      struct sockaddr_in peer;
      peer.sin_family=AF_INET;
      peer.sin_port=htons(port);
      peer.sin_addr.s_addr=inet_addr(ip.c_str());
      for(;;){
        std::cout<<"Please Enter# ";
        std::cin >> msg;
        if(msg=="quit"){//最好使用组合键退出
          break;
        }

        sendto(sock,msg.c_str(),msg.size(),0,(struct sockaddr*)&peer,sizeof(peer));

        char echo[128];//用来记录接收的数据
        ssize_t s=recvfrom(sock,echo,sizeof(echo)-1,0,nullptr,nullptr);//谁給我发的可以不关心，因为数据不需要回馈
        //如果想关心定义strut sockaddr_in temp;用这个来记录便可
        if(s>0){
          echo[s]=0;
          std::cout<<"server# "<<echo<<std::endl;
        }
      }
    }

    ~udpClient(){
      close(sock);//关掉文件描述符
    }
};
