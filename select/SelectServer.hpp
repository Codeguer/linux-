#include"Sock.hpp"

#define DFL_PORT 8080
#define NUM (sizeof(fd_set)*8)
#define DFL_FD -1

class SelectServer{
  private:
    int lsock;
    int port;
    int fd_array[NUM];//辅助数组
  public:
    SelectServer(int _p=DFL_PORT):port(_p)
    {}
    void InitServer(){
      for(int i=0;i < NUM;i++){
        fd_array[i]=DFL_FD;//-1表示非法
      }
      lsock=Sock::Socket();//创建套接字
      Sock::Setsockopt(lsock);//将端口号设置为复用状态,需要在Bind之前设置
      //因为必须在Bind之前告诉新的套接字这个端口号可以复用，不然不知道
      Sock::Bind(lsock,port);//绑定IP地址与端口号
      Sock::Listen(lsock);//将套接字设置为监听状态

      fd_array[0]=lsock;
    }

    void AddFd2Array(int sock){
      //这里要把新的文件描述符放进fd_array中
      //因此需要寻找fd_array中未被使用的位置j
      int i=0;
      for(;i < NUM;i++){
        if(fd_array[i]==DFL_FD){
          break;
        }
      }
      
      //判断fd_array是否满了
      if(i==NUM){
        cerr<<"fd_array is full,so close the sock" <<endl;
        close(sock);
      }
      else{
        fd_array[i]=sock;
        cout<<"fd: "<<sock<<" add to select ..."<<endl;
      }
    }
    void DefFdFromArray(int index){
      if(index >=0 && index < NUM){
       fd_array[index]=DFL_FD; 
      }
    }
    void HanlderEvents(fd_set *rfds){
      //比如用户想让关心的文件描述符有1,3,5,7,8,9
      //rfds作为输出时保存的文件描述符是其子集，比如1,3或者1,7等等
      //那么问题来了，你怎么知道是哪些文件描述符就绪了呢？因此需要遍历rfds
      //目前已经有了最大的maxfd，那不就是从0开始遍历到maxfd，然后检测rfds里有木有不就可以了吗？
      //但会带来一个问题：比如我只打开了1，3，5，7文件描述符，因此只需要遍历这些合法的就可以了
      //如何从0开始遍历那就将那些非法的即没有打开的也遍历了一遍,遍历非法的没必要也不需要）
      //因此这里构建一个辅助数组，将已经打开的文件描述符全部保存起来
      
      for(int i=0; i < NUM;i++){
        if(fd_array[i]==DFL_FD){
          //为-1就说明是非法的就不要检测该文件描述符是否就绪了
          continue;
        }

        //这里就是有效的文件描述符，然后检测是否在rfds，因为rfds作为输出保存了已经就绪的文件描述符
        //检测是否在rfds其实就是检测该文件描述符是否就绪了
        
        //检测是否在rfds位图中，用FD_ISSET函数
        if(FD_ISSET(fd_array[i],rfds)){
          //read ready
          //这里注意一个问题：比如fd_array保存了10个文件描述符，其中我只关心前五个文件描述符读事件是否就绪
          //因此将前五个设置进rfds，那么当rfds作为输出时我遍历fd_array数组时rfds之中是绝对不会出现后五个文件描述符的
          //当然这种情况是很少见的，因为这10个都是读文件描述符，一般都设置到rfds中（如果要关心写文件描述符应该再
          //建立一个辅助数组专门来保存写文件描述符,
          
          //因为文件描述符的功能不同因此要进行分类，比如普通的读事件就绪了还有底层有新的链接这也是读事件就绪
          
          if(fd_array[i]==lsock){//lsock是监听套接字，它专门获取链接的（获取到链接就说明读事件就绪
            //link event
            int sock=Sock::Accept(lsock);//这里就不会被阻塞了，因为事件已经就绪了
            if(sock>=0){
              //sock OK
              //这里还有一个问题，获取到新的链接能不能里面用recv或read取获取数据呢？
              //不能，因为链接建立好但是如果没有数据即读事件未就绪那么recv或read就会阻塞，而且该
              //服务器还是单进程服务器就会被挂起，对外表现的好像和卡死一样
              //因此该链接也需要被管理起来即放到fd_array中，
              cout<<"get a new link ..."<<endl;
              AddFd2Array(sock);//将新文件描述符放进fd_array中
            }
            
          }
          else{
            //Data read event
            //ssize_t recv(int sockfd, void *buf, size_t len, int flags);
            //等的过程已经完了，，这里直接进行拷贝，拷贝是把数据从内核到用户，这里的
            //buf就是用户自己定义的用户缓冲区，flags默认设置为0，因为等的过程完成了
            //所以flags为0不会使recv阻塞等待
            //
            //len表示期望读取多少个，比如1024个，如果数据只有100个数据不够怎么办,那么就通过返回值将实际读取的字节数
            //保存起来，并不会被阻塞住
            //
            //如果出现只有读完1024个字节才能进行数据处理，比如读http协议报头，只有把把头读完了才能进行处理
            //那么这里可以对读上来的100字节先不做处理，等读够一定长度就开始进行处理
            //因此使用recv时除了考虑实际读到多少内容还要考虑具体的协议或者业务
            
            //下面这样处理有bug的
            char buf[1024];
            ssize_t s=recv(fd_array[i],buf,sizeof(buf),0);
            if(s > 0){
              buf[s]=0;
              cout<< "client# "<<buf<<endl;
            }
            else if(s ==0){
              //s为0表示对方把链接关了,那我也要关
              cout<<"client quit"<<endl;
              close(fd_array[i]);//现在fd_array数组里面还是有这个文件描述符，因此需要对该数组做处理
              DefFdFromArray(i);
            }
            else{
              //报错了,那就把这个文件描述符关掉，这里的处理动作与上面一样
              cout<<"client quit"<<endl;
              close(fd_array[i]);
              DefFdFromArray(i);
            }
          }
        }
        //当然如果你还想对同一组文件描述符关心读事件当然不要再构建专门保存写文件描述符的辅助数组
      //  if(FD_ISSET(fd_array[i]),wfds){
      //    //write ready
      //  }
      }
      
      
    }
    void Start(){
     // int maxfd=lsock;//目前只有lsock文件描述符（即使你考虑三标准也是lsock值是最大的）
      //有一种进程为守护进程，它是会关闭文件描述符的0，1，2的所有lsock也可能是0，不过影响不到这里
      //因为这种写法的进程不是守护进程
    //  struct timeval timeout={5,0};//timeval是个结构体，自然可以定义变量并初始化,将该语句写到下方
    //  struct timeval {
    //     long    tv_sec;         /* seconds */
    //     long    tv_usec;        /* microseconds */
    //  };
      int maxfd=DFL_FD;//因为有新的文件描述符被打开因此maxfd也要变化，这里先初始化为-1 

      for(;;){
        //select
        fd_set rfds;//fd_set是用结构体实现的位图，用fd_set定义一个变量，该变量是在栈上保存的，因此其值是随机值
        //清除描述词组set的全部位
        FD_ZERO(&rfds);//这里不建议用memset函数来对位图进行设置
        cout<<"fd_array：";
       // FD_SET(lsock,&rfds);//将lsock文件描述符设置进位图中
       
        for(int i=0; i < NUM;i++){//将只添加lsock修改为将fd_array中的合法的文件描述符都添加进来
          if(fd_array[i]!=DFL_FD){
            cout<<fd_array[i]<<" ";
            FD_SET(fd_array[i],&rfds);
            //更新maxfd
            if(maxfd < fd_array[i]){
              maxfd=fd_array[i];
            }
          }
        }
        cout<<endl;

        cout<<"begin select ..."<<endl;
        struct timeval timeout={50,0};//每次调用select都需要对timeout重新设置
        switch( select(maxfd+1,&rfds,nullptr,nullptr,&timeout)){//写事件与错误事件不关心设置为nullptr就行
          //timeout既是输入也是输出，你传进去的是5秒，当select返回的时候如果没有事件就绪timeout就会被设置为0被返回
          case 0:
            //返回0代表在描述词状态改变前已超过timeout时间，没有就绪事件
            //printf("timeout ...\n");
            cout<<"timeout ..."<<endl;
            break;
          case -1:
            //error,一般不会出错
            //perror("select error!\n");
            cerr<<"select error!"<<endl;
            break;
          default:
            //success
            //有一个文件描述符就绪了就可以处理这个文件描述符了
            //这里至少有一个文件描述符就绪了即读事件就绪了，等的过程完成了就要开始拷贝工作
            //当rfds作为输入时代表用户想让操作系统关心这写文件描述符是否就绪
            //当rfds作为输出时代表已经就绪的文件描述符保存在rfds中
            HanlderEvents(&rfds);//因此这里传rfds就可以了
            break;
        } 
      }
    }

    ~SelectServer(){

    }
};
