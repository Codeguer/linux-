#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include<iostream>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

class tcpClient{
  private:
    int svr_port;
    std::string svr_ip;
    int sock;
  public:
    tcpClient(std::string _ip="127.0.0.1",int _port=8080)
      :svr_ip(_ip),svr_port(_port)
    {}
    void initClient(){
      sock=socket(AF_INET,SOCK_STREAM,0);
      if(sock < 0){
        std::cerr<<"socket error"<<std::endl;
        exit(2);
      }
      //客户端TCP与UDP一样不需要绑定
      //设置为监听状态是为了检测是否有人连我，因此客户端不需要监听，客户端是主动连服务器的
      //客户端不需要监听因此也就无法使用accept获取链接，accept函数的第一个参数就是监听状态的套接字
      
      //客户端使用connect进行连接服务器的操作
      struct sockaddr_in svr;
      svr.sin_family=AF_INET;
      svr.sin_port=htons(svr_port);
      svr.sin_addr.s_addr=inet_addr(svr_ip.c_str());

      //客户端使用connect进行连接的同时操作系统在底层就自动帮客服端完了IP地址与端口号的分配以及绑定
      if(connect(sock,(struct sockaddr*)&svr,sizeof(svr)) != 0){
        std::cerr<<"connect error"<<std::endl;
      }
      //connect success

    }
    void start(){
      char msg[64];
     // while(true){//进行一次操作就退出,因为服务端完成一个任务就关闭该链接，因此不需要while循环
        std::cout<<"Please  Enter Message# ";
        fflush(stdout);//因为有用户层的C++缓冲区因此需要强制刷新
        size_t s=read(0,msg,sizeof(msg)-1);//0叫做标准输入，从0那里读,读的数据放到msg
        if(s>0){
         // msg[s]=0;
          msg[s-1]=0;//解决将回车符读取进来的问题,在这里将回车符给去掉了
          send(sock,msg,strlen(msg),0);//阻塞默认为0
          size_t ss=recv(sock,msg,sizeof(msg)-1,0); 
          if(ss > 0){
            msg[ss]=0;
            std::cout<<"server echo # "<<msg<<std::endl;
          }
          else if(ss == 0){//判断服务端是否关闭了
           // break;//服务器将该链接关闭那么这里就会读到0
          }
          else{
            //break;
          }
        }
     // } 
    }
    ~tcpClient(){
        close(sock);
    }
};

#endif
