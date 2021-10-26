#ifndef __TCP_SERVER_H
#define __TCP_SERVER_H

#include<iostream>
#include<string>
#include<unistd.h>
#include<cstring>
#include<cstdlib>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<pthread.h>
#include"ThreadPool.hpp"

#define BACKLOG 5

//using namespace std;//尽量放到源文件里，如果放到头文件别人一包含可能就会有命名冲突
class tcpServer{
    private:
      int port;
      int listen_sock;//用来拉客的（客人就是链接),监听套接字
      ThreadPool *tp;
    public:
      tcpServer(int _port):port(_port),listen_sock(-1),tp(nullptr)
      {}
      void initServer(){
        signal(SIGCHLD,SIG_IGN);//让主进程忽略该信号
        listen_sock=socket(AF_INET,SOCK_STREAM,0);
        if(listen_sock < 0){
          std::cerr<<"socket error"<<std::endl;//错误信息应该往日志里放，可以使用第三方日志库比如log4cxx
          //如果不用日志库可以执行服务器的时候使用追加重定向到文件
          exit(2);
        }
        struct sockaddr_in local;
        local.sin_family=AF_INET;
        local.sin_port=htons(port);
        local.sin_addr.s_addr=INADDR_ANY;
        //或写成这样也行，INADDR_ANY本身是0值无所谓大小端
       // local.sin_adddr.s_addr=htonl(INADDR_ANY);
        
        //使用bind绑定是为了确定服务器在哪的问题
        if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local)) < 0){
            std::cerr<<"bind error"<<std::endl;
            exit(3);
        }

        //必须让套接字设置为监听状态：便能知道是否有客户端过来链接服务器（
        if(listen(listen_sock,BACKLOG) < 0){
          std::cerr<<"bind error"<<std::endl;
          exit(4);
        }
        tp=new ThreadPool();
        tp->ThreadPoolInit();
      }
    static  void service(int sock){//service也可以写为static静态函数
        char buffer[1024];
        while(true){
          //这里可以使用read or write
          //这里使用recv代替read，recv功能比read更多，两者返回值一模一样
          size_t s=recv(sock,buffer,sizeof(buffer)-1,0);
         // size_t s=read(sock,buffer,sizeof(buffer)-1);//与recv效果一样
          if(s > 0){
            buffer[s]=0;
            std::cout<<"client# "<<buffer<<std::endl;

          //这里使用send代替write
            send(sock,buffer,strlen(buffer),0);//strlen(buffer)不要加1，因为网络是文件，不认识'\0'
          }
         else if(s == 0){//当等于0时说明客户端被关闭了
            std::cout<<"client quit ... "<<std::endl;
            break;
          }
          else{
            //读取失败,这种情况现在很少见
            std::cout<<"recv client data error..."<<std::endl;
            break;
          }
        }
        close(sock);//close的头文件为unistd.h，如果不关闭文件描述符的话意味着你可使用的文件描述符越来越少
        //一个进程所使用的文件描述符是有上限的，默认32个，不过内部有拓展表，因此实际可以打开的数量比32大的多
      }

  //     static void* serviceRoutine(void *arg){//加static是为了去掉this指针
  //       pthread_detach(pthread_self());//线程分离，线程结束自动释放从而不会内存泄漏，不要让主线程进行pthread_join()阻塞等待该线程
  //        std::cout<<"create a new thread for IO"<<std::endl;
  //        int* p=(int *)arg;
  //       // int sock=*(int *)arg; 
  //        int sock=*p; 
  //        service(sock);//service必须为静态函数方可被这里调用
  //        delete p;
  //     }

      void start(){
        sockaddr_in endpoint;
        while(true){
          socklen_t len=sizeof(endpoint);
          int sock=accept(listen_sock,(struct sockaddr*)&endpoint,&len);//accept获取链接(拉客)将排队的客户端拿上来，用sock进行服务
          if(sock < 0){
            std::cerr<<"accept error"<<std::endl;
            continue;//如果accept拉客失败则重新拉下一个
          }
          //记录客户端的信息
          std::string cli_info=inet_ntoa(endpoint.sin_addr);//将4字节的无符号整型IP转为主机序列的点分十进制形式的字符串
          cli_info+=":";
          cli_info+=std::to_string(ntohs(endpoint.sin_port));//要讲端口号也转为字符串，先将端口号转为转为主机序列，再用to_string函数
          
         // std::cout<<"get a new link ..."<<std::endl;
          std::cout<<"get a new link ..."<<cli_info<<" sock: "<<sock<<std::endl;

          //Task t(sock);//在栈上创建r任务的话因为传给Put函数传的是引用且任务队列里放的是执行任务的指针，会导致任务出错，因此改为在堆上创建
          //tp->Put(t);
          Task *t=new Task(sock);
          tp->Put(*t);//传入任务队列，当某一个线程完成了该任务就关闭该链接
     //     pthread_t tid;//新线程与主线程共享文件描述符表，因此不能想使用子进程的方法一样关闭listen_sock
     //     int *p=new int(sock);//申请堆空间时只有一个执行流，所以没问题
     //     //pthread_create(&tid,nullptr,serviceRoutine,(void*)&sock);//直接传sock是有BUG的
     //     pthread_create(&tid,nullptr,serviceRoutine,(void*)p);//传的是指针p的拷贝，因此主线程将p给更新了也没关系，影响不到这里

    //      //创建子进程去完成服务，主进程继续拉客
    //      pid_t id=fork();
    //      if(id == 0){
    //        //父进程有的子进程也有
    //        close(listen_sock);//子进程不关心listen_sock,关心的是lock,推荐这里关闭掉(因为用户总有捣蛋的),当然这不会影响主进程，文件描述虽然一样，
    //        //也指向同一份文件，但两进程的文件描述符表不一样，所以不会影响主进程
    //         service(sock);//进行服务
    //         exit(0);//子进程退出
    //      }
    //      //waitpid(id,NULL,0);//如果以阻塞方式等待是不行的，因为主进程就无法马上去拉客了
    //      //设置为非阻塞不推荐
    //      //如果不等待子进程一旦退出就变成僵尸进程了也不可取
    //      //注册SIGCHLD信号，因为子进程退出时会向父进程发SIGCHLD信号，如果发了该信号就调用wait将子进程回收了,这操作不简单
    //      //使父进程忽略SIGCHLD信号，即将SIGCHLD的处理动作置为SIG_IGN,这样fork出来的子进程在终止时会系统自动清理掉,不会产生僵尸进程,也不会通知父进程
    //      
    //      close(sock);//父进程不关心sock,且父进程必须要关,因为当子进程越来越多，那么父进程可用的文件描述符越来越少
        }
      }
      ~tcpServer(){
      }
};

#endif
