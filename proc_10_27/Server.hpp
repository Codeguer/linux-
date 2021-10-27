#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include"Protocol.hpp"

using namespace std;
class server{
  private:
    int port;
    int lsock;
  public:
    server(int _port):port(_port),lsock(01)
   {}
    void initServer(){
      lsock=socket(AF_INET,SOCK_STREAM,0);
      if(lsock < 0){
        cerr<<"socket error "<<endl;
        exit(2);
      }
      struct sockaddr_in local;
      local.sin_family=AF_INET;
      local.sin_port=htons(port);
      local.sin_addr.s_addr=INADDR_ANY;

      if(bind(lsock,(struct sockaddr*)&local,sizeof(local)) < 0){
        cerr<<" bind error!"<<endl;
        exit(3);
      }

      if(listen(lsock,5) < 0){
        cerr<<"listen error!\n"<<endl;
      }
    }

    void cal(int sock){
      //将cal设计为基于短链接来完成对应的计算
      //短链接即客户端发起一个请求，服务器响应处理后返回给客户端，一个服务周期就完了，服务器把该链接断开
      request_t rq;
      response_t rsp={4,0};
      ssize_t s=recv(sock,&rq,sizeof(rq),0);
      if(s > 0){
        rsp.code=0;
        switch(rq.op){
          case '+':
            rsp.result=rq.x+rq.y;
            break;
          case '-':
            rsp.result=rq.x-rq.y;
            break;
          case '*':
            rsp.result=rq.x*rq.y;
            break;
          case '/':
            if(rq.y != 0){
               rsp.result=rq.x/rq.y;
            }
            else{
               rsp.code = 1;
            }
            break;
          case '%':
            if(rq.y != 0){
               rsp.result=rq.x/rq.y;
            }
            else{
               rsp.code = 2;
            }
            rsp.result=rq.x+rq.y;
            break;
          default:
            rsp.code=3;
            break;
        }
      }
      //这里先不考虑s==0即客户端被关闭的情况,因为基于短链接，客户端先发，然后服务器再收
      
      send(sock,&rsp,sizeof(rsp),0);
      close(sock);
    }

    void start(){
      struct sockaddr_in peer;
      for(;;){
        socklen_t len=sizeof(peer);
        int sock=accept(lsock,(struct sockaddr*)&peer,&len);
        if(sock < 0){
          cerr<<"accept error!"<<endl;
          continue;
        }
        if(fork()==0){//终止子进程
          if(fork() > 0){
            exit(0);
          }
          //孙子
          close(lsock);
          cal(sock);
          exit(0);
        }
        close(sock);
        waitpid(-1,nullptr,0);//如果传入的pid是非负数那么是指定的进程，如果是-1代表是任意一个等价于wait
        //nullptr表示不关心其返回值，0表示以阻塞方式等待
        //注意在这里父进程不会被阻塞住，因为子进程创建孙子进程后就马上被终止了,父进程马上就把子进程给回收了
      }
    }
    ~server(){
      close(lsock);
    }
};

#endif
