#pragma once

#include<iostream>
#include<queue>
#include<map>
#include<string>
#include<math.h>
#include<unistd.h>
#include<math.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<cstring>

#define NUM 5  //一般创建线程的个数和我们当前的CPU个数以及CPU的核数是有关系的

class Task{
  public:
   // int base;
    int sock;//用于把计算的结果写回客户端
    std::map<std::string,std::string> dict;
  public:
    Task(){}
    Task(int _sock):sock(_sock){
      dict.insert(std::pair<std::string,std::string>("apple","苹果"));
    }
    void Run(){
        char buffer[64];
        std::cout<<"task is running "<<std::endl;
        // std::cout<<"thread is["<<pthread_self()<<"] task run ... done:base# "<<base<<" pow is# "<<pow(base,2)<<std::endl;
        size_t s = recv(sock, buffer, sizeof(buffer) - 1, 0);
        //size_t s=read(sock,buffer,sizeof(buffer)-1);//与recv效果一样
        if (s > 0) {
           buffer[s] = 0;
           std::cout << "client# " << buffer << std::endl;
           std::string key=buffer;

           //这里使用send代替write
          // send(sock, buffer, strlen(buffer), 0);//strlen(buffer)不要加1，因为网络是文件，不认识'\0'
           send(sock,dict[key].c_str(),dict[key].size(),0);
        }
        else if (s == 0) {//当等于0时说明客户端被关闭了
           std::cout << "client quit ... " << std::endl;
        }
        else {
           //读取失败,这种情况现在很少见
           std::cout << "recv client data error..." << std::endl;
        }
    }
    ~Task(){
      std::cout<<"server close sock :"<<sock<<std::endl;
      close(sock);
    }
};

class ThreadPool{
  private:
    std::queue<Task*>q;//当任务量过大时拷贝任务就成了问题，因此用指针
    int max_num;
    pthread_mutex_t lock;
    pthread_cond_t cond;//这里不会让生产者等，因为生产者不仅仅要将任务放进任务队列里
    //还要从网络中获取任务，一旦让生产者等（即任务队列满了），那么用户就无法链接到该服务器了
    //因此任务队列是没有上限的，如果任务队列满了，要么通过软件优化要么买服务器
    //让任务队列能够承载其业务，
  //  bool quit;//这里不考虑退出
  public:
    void LockQueue(){
        pthread_mutex_lock(&lock); 
    }
    void UnLockQueue(){
        pthread_mutex_unlock(&lock);
    }
    bool IsEmpty(){
      return q.size()==0;
    }
    void ThreadWait(){
      pthread_cond_wait(&cond,&lock);
    }
    void  ThreadWakeup(){
      pthread_cond_signal(&cond);//cond是对某个条件的抽象，它里面必定包含一个等待队列
      //这里的五个线程都在里面等，用该函数唤醒在等待队列里的第一个线程k
      //与此相生的函数int pthread_cond_broadcast(pthread_cond_t *cond);是唤醒
      //处于该等待队列的所有线程，不过在这里用没什么意义，因为这几个线程是互斥的
    }
    void ThreadsWakeup(){
       pthread_cond_broadcast(&cond); 
    }
  public:
    ThreadPool(int _max=NUM):max_num(_max){
      //将有风险的事通过ThreadPoolInit来做，确保对象一定创建成功
    }
   static  void *Routine(void*arg){
      ThreadPool *this_p=(ThreadPool*)arg;//接受传入的this对象
      //线程池中的线程默认使其永远不退出
      while(true){
        //保护临界资源
       this_p-> LockQueue();
       while( this_p->IsEmpty()){
         this_p-> ThreadWait();
        }
       Task* t;
       this_p-> Get(&t);
       this_p-> UnLockQueue();
       t->Run();
        //t.Run();//处理任务要花时间，在临界区已经把这个任务拿走了，这个任务就属于该线程私有的了
        //也就是说分配好的任务已经不属于临界资源了，因此处理任务就不需要放在临界区里了，
        //这样可以快速的让其它线程进来获取任务，提高效率
       delete t;//任务跑完就删掉,调用析构函数将该链接关闭
      }
    }
    void ThreadPoolInit(){
      pthread_mutex_init(&lock,nullptr);
      pthread_cond_init(&cond,nullptr);
      pthread_t t;

      for(int i = 0; i < max_num;i++){
        pthread_create(&t,nullptr,Routine,this);//Routine函数如果有this指针,那么该函数就会有两个参数
        //pthread_create的回调函数默认带一个参数，从而就会出现错误，因此需要将Routine函数改为静态成员函数，但是Routine因为没有this指针又出现无法
        //调用其它成员函数的情况，因此需要显示传入this指针，然后显示调用其它成员函数，因此其它成员函数必须是
        //公有的，不然无法调用它们
      }
    }
    //server
    void Put(Task &in){
      LockQueue();
      q.push(&in);
      UnLockQueue();

      ThreadWakeup();
    }
    //Thread pool t
    void Get(Task **out){
      //因为该函数已经被锁锁住了，因此这里直接拿
      Task*t=q.front();
      q.pop();
     * out=t;
    }
    void ThreadQuit(){
//      if(!IsEmpty()){
//        std::cout<<"task queue is not empty()"<<std::endl;
//        return ;
//      }
//      quit=true;
//      ThreadsWakeup();
    }
    ~ThreadPool(){
      pthread_mutex_destroy(&lock);
      pthread_cond_destroy(&cond);
    }
};
