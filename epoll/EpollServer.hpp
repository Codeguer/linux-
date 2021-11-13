#include"Sock.hpp"

#define SIZE 64

struct bucket{
  char buffer[20];//当然这个buffer你也可以写成某一种request来定义协议
  int pos;
  int pos1;
  int fd;

  bucket(int sock):fd(sock),pos(0),pos1(0){
    memset(buffer,0,sizeof(buffer));

  }
  ~bucket(){

  }
};

class EpollServer{
    private:
      int lsock;
      int port;
      int epfd;
    public:
      EpollServer(int _p=8080):port(_p){

      }
      void AddEvent2Epoll(int sock,uint32_t event){
        struct epoll_event ev;
        ev.events=event;
        if(sock==lsock){
          ev.data.ptr=nullptr;//ptr为空则表明这是监听套接字,否则就是正常的套接字
        }
        else{
          ev.data.ptr=new bucket(sock);//正常套接字所对应的用户缓冲区
        }
        epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&ev);
        //EPOLL_CTL_ADD:注册新的fd到epfd中
      }
      void DelEventFromEpoll(int sock){
         close(sock);
         epoll_ctl(epfd,EPOLL_CTL_DEL,sock,nullptr);
      }
      void InitServer(){
        lsock=Sock::Socket();
        Sock::Setsockopt(lsock);
        Sock::Bind(lsock,port);
        Sock::Listen(lsock);

        epfd=epoll_create(256);//创建epoll模型
        if(epfd < 0){
          cerr<<"epoll_create error"<<endl;
          exit(5);
        }
        cout<<"listen sock: "<<lsock<<endl;
        cout<<"epoll fd: "<<epfd<<endl;
      }
      void HandlerEvent(struct epoll_event revs[],int num){
        for(int i=0;i < num;i++){
          u_int32_t ev=revs[i].events; 
          if(ev&EPOLLIN){
            //read
            if(revs[i].data.ptr!=nullptr){
               bucket *bp=(bucket*)revs[i].data.ptr;
               ssize_t s=recv(bp->fd,bp->buffer+bp->pos,sizeof(bp->buffer)-bp->pos,0);
               if(s>0){
                 bp->pos+=s;
                 cout<<"client# ..."<<bp->buffer<<endl;
                 if(bp->pos>=sizeof(bp->buffer)){
                   //当读到足够的字节数就开始写
                   //将读事件改为写事件
                   struct epoll_event temp;
                   temp.events=EPOLLOUT;
                   temp.data.ptr=bp;
                   epoll_ctl(epfd,EPOLL_CTL_MOD,bp->fd,&temp);
                 }
                 
               }
               else if(s == 0){
                 //s==0说明对方把链接断开了
                 DelEventFromEpoll(bp->fd);
                 delete bp;
               }
               else{
                 //read error

               }
              

            }
            else{
              //listen sock;
              int sock=Sock::Accept(lsock);
              if(sock>0){
                //将新链接所关心的事件添加到epoll模型当中
                AddEvent2Epoll(sock,EPOLLIN);
              }
            }

          }
          else if(ev&EPOLLOUT){
            //write
            bucket *bp=(bucket*)revs[i].data.ptr;

            //这里也有和recv一样的问题，你无法保证send一次发送的字节数足够
            //解决方法和recv一样
            ssize_t s=send(bp->fd,bp->buffer+bp->pos1,sizeof(bp->buffer)-bp->pos1,0);
            bp->pos+=s;
            if(bp->pos>=sizeof(bp->buffer)){
              //确认发送的字节数足够了就断开链接
               DelEventFromEpoll(bp->fd);
               delete bp;
            }
          }
          else{
            //other events
          }
        }
      }
      void Start(){
        //将lsock所关心的事件添加到epoll模型当中
        AddEvent2Epoll(lsock,EPOLLIN);//设置好后用户就可以让操作系统关心
        //lsock的读事件
        int timeout=-1;
        struct epoll_event revs[SIZE];
        for(;;){
          int num=epoll_wait(epfd,revs,SIZE,timeout);//将已经就绪的文件描述符以epoll_event的结构体
          //形式保存在revs中
          //如果有500个就绪了，我revs最大也才64怎么办？不影响，一次拿不完就多次拿
          //如果只有6个就绪了，没拿够怎么办？可以通过num返回值来确定实际拿了多少个
          //如果6个就绪了，但是注册了十个文件描述符，如何知道是哪6个文件描述符呢
          //所有就绪的文件描述符及其事件最终通过revs返回时它会从下标0处依次填入已经就绪的事件
          //将来我想知道哪些文件描述符就绪了只需要从0开始到num遍历revs就可以了
          
          switch(num){
            case 0:
              cout<<"time out ..."<<endl;
              break;
            case -1:
              cerr<<"epoll_wait error"<<endl;
            default:
              HandlerEvent(revs,num);
              break;
          }
        }
      }
      ~EpollServer(){
        close(lsock);
        close(epfd);
      }
};
