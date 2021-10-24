#pragma once

#include<iostream>
#include<cstdio>
#include<string>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<map>

class udpServer{
  private:
   // std::string ip;//一般服务器不这样写
    int port;
    //这里的ip与port是在用户层创建出来的
    int sock;
    std::map<std::string,std::string>dict;
  public:
    udpServer(int _port=8080)//127.0.0.1为本地环回，使用本地环回可以进行代码测试
      //即用我们对应的client来连我的server,在一台机器上实现通信就可以
      :port(_port)
    {
      //可以将字典信息放到文件里
      dict.insert(std::pair<std::string,std::string>("apple","苹果,是一种水果!"));
      dict.insert(std::pair<std::string,std::string>("banana","香蕉,是一种水果!"));
      dict.insert(std::pair<std::string,std::string>("student","学生"));
    }
    void initServer(){
      sock=socket(AF_INET,SOCK_DGRAM,0) ;//套接字是在内核层创建出来的
      std:: cout<<"sock: "<<sock<<std::endl;//sock默认是3
      struct sockaddr_in local;//传递ip与port需要借助sockaddr_in结构体
      local.sin_family=AF_INET;
      local.sin_port=htons(port);//用htons保证是网络序列
     // local.sin_addr.s_addr=inet_addr(ip.c_str());//sin_addr是一个结构体对象，对象里的s_addr成员才是IP地址
      //我们定义的IP地址是字符串，需要进行转换为网络序列的4字节整数
      
      //使用INADDR_ANY
      local.sin_addr.s_addr=INADDR_ANY;


      //C++有cin,cout,cerr,和系统的0,1,2对应，和C语言的stdin,stdout,stderr
      //cerr就是向标准错误输出，cerr与cout虽然都是往显示器输出，但是往不同的文件描述符里打
      if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
        std::cerr<<"bind error!\n"<<std::endl;
        exit(1);
      }
    }
    //让服务器跑起来
    void start(){
      char msg[64];
      //服务器跑起来一般不会停止，因此使用死循环
      for(;;){
        //echo server:回应服务器
        //让该服务器从网络中接收数据，然后把收到的数据在本地打印一下
        //并且将收到的数据进行内容增加后将其返回回去
        msg[0]='\0';
        struct sockaddr_in end_point;
        socklen_t len=sizeof(end_point);
        ssize_t s=recvfrom(sock,msg,sizeof(msg)-1,0,(struct sockaddr*)&end_point,&len);
        if(s>0){
          char buf[16];
          sprintf(buf,"%d",ntohs(end_point.sin_port));//将整型转为字符串，linux没有itoa函数
          std::string cli=inet_ntoa(end_point.sin_addr);//放end_point中的结构体变量成员
          cli+=":";
          cli+=buf;

          msg[s]='\0';//目的是让其以C字符串的形式进行输出
        //  std::cout<<"client#"<<msg<<std::endl;
          std::cout<<cli<<"#"<<msg<<std::endl;
         // std::string echo_string=msg;
          std::string echo="unknow";
          auto it=dict.find(msg);
          if(it!=dict.end()){
              echo=dict[msg];
          }
         // echo_string +=" [server echo!]";
        //  sendto(sock,echo_string.c_str(),echo_string.size(),0,(struct sockaddr*)&end_point,len);
          sendto(sock,echo.c_str(),echo.size(),0,(struct sockaddr*)&end_point,len);
        }
      }
    }

    ~udpServer(){
      close(sock);//关掉文件描述符
    }
};
