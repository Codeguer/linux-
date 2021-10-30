#pragma once

#include<iostream>
#include<string>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

#define BACKLOG 5

using namespace std;

class HttpServer{
  private:
    int port;
    int lsock;
  public:
    HttpServer(int _p):port(_p),lsock(-1)
    {}
  
    void InitServer(){
      signal(SIGCHLD,SIG_IGN);
      lsock=socket(AF_INET,SOCK_STREAM,0);
      if(lsock < 0){
        cerr<<"socket error"<<endl;        
        exit(2);
      }
      struct sockaddr_in local;
      bzero(&local,sizeof(local));//对一段缓冲区（第一个参数）进行清零
      local.sin_family=AF_INET;
      local.sin_port=htons(port);
      local.sin_addr.s_addr=INADDR_ANY;

      if(bind(lsock,(struct sockaddr*)&local,sizeof(local)) < 0){
        cerr<<"socket bind error"<<endl; 
        exit(3);
      }

      if(listen(lsock,BACKLOG) < 0){
        cerr<<"listen error"<<endl;
        exit(4);
      }
    }

    void EchoHttp(int sock){
      char request[2048];
      size_t s=recv(sock,request,sizeof(request),0);//这里有bug，因为http基于tcp，
      //tcP面向字节流的，因此可能会出现一个报文分多次才能发完，UDP要么没发要么一次就发完
      //所以需要通过上层去识别有没有把报文(包括首行到正文),应用层就需要做协议处理
      //因为这里的http请求量不大，recv能够一次把请求接收完
      if(s > 0){
        request[s]=0;
        cout<<request<<endl;

        //响应首行
        string response="HTTP/1.0 302 NOT Found\r\n";//不写\r也没问题
        //响应报文
        response+="Content-type: text/html\r\n";
        response += "location: https://www.baidu.com";//搭配3xx状态码使用, 告诉客户端接下来要去哪里访问;
        //空行
        response+="\r\n";
        //响应正文
      //  response += "\
      //    <!DOCTYPE html>\
      //    <html>\
      //    <head>\
      //    <title>MyServer</title>\
      //    </head>\
      //    <body>\
      //    <h1>Welcome</h1>\
      //    <p>HelloWorld</p>\
      //    </body>\
      //    </html>\
      //    ";
         send(sock,response.c_str(),response.size(),0);
      } 
      close(sock);//基于短链接
    }

    void Start(){
      struct sockaddr_in peer;
      for(;;){
        socklen_t len=sizeof(peer);
        int sock=accept(lsock,(struct sockaddr*)&peer,&len);
        if(sock < 0){
          cerr<<"accept error"<<endl;
          continue;
        }

        cout<<"get a new connect ... done"<<endl;

        if(fork()==0){
          //child
          close(lsock);
          EchoHttp(sock);
          exit(0);
        }
        close(sock);
      }
    }
    ~HttpServer(){
      if(lsock!=-1){
        close(lsock);
      }
    }

};
