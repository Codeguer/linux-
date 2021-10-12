#ifndef __QUEUE_BLOKC_H__
#define __QUEUE_BLOKC_H__

#include<iostream>
#include<pthread.h>
#include<queue>
#include<unistd.h>

class Task{
  public:
    int x;
    int y;
  public:
    Task()
    {}
    Task(int _x,int _y):x(_x),y(_y)
    {}
    int Run(){
      return x+y;
    }
    ~Task()
    {}
};

class BlockQueue{
  private:
    std::queue<Task>q;
    size_t cap;//BlockQueue队列的容量大小
    pthread_mutex_t lock;
    pthread_cond_t c_cond;//将来消费者，在该条件变量下等
    pthread_cond_t p_cond;//将来生产者，在该条件变量下等
  public:
    bool IsFull()
    {
      return q.size()>=cap;//其实只要q.size()==cap就行了，不过写成>=是一种好的习惯
      //由一个数字变成一群数字来判断该条件错误率就会下降
    }
    bool IsEmpty()
    {
      return q.empty();
    }
    void LockQueue()
    {
      pthread_mutex_lock(&lock); 
    }
    void UnLockQueue()
    {
      pthread_mutex_unlock(&lock);
    }
    void WakeUpComsumer()
    {
      std::cout<<"wake up consumer..."<<std::endl;
      pthread_cond_signal(&c_cond); 
    }
    void WakeUpProductor()
    {
      std::cout<<"wake up productor..."<<std::endl;
      pthread_cond_signal(&p_cond);
    }
    void ProductorWait()
    {
      std::cout<<"productor wait..."<<std::endl;
      pthread_cond_wait(&p_cond,&lock);
    }
    void ComsumerWait()
    {
      std::cout<<"comsumer wait..."<<std::endl;
      pthread_cond_wait(&c_cond,&lock);
    }
  public:
    BlockQueue(size_t _cap):cap(_cap)
   {
     //这种写法其实不太好，在构造函数里最好不要做一些可能会失败的事情
     //比如进行了new申请空间，因为一旦new失败了，那么构造也就失败了
     //那么对象创建出来了还是没有创建出来呢
     pthread_mutex_init(&lock,nullptr);  
     pthread_cond_init(&c_cond,nullptr);
     pthread_cond_init(&p_cond,nullptr);
   }

    //未来可能有多个线程进行put与get操作因此要加锁
    void Put(Task t)
    {
      LockQueue();
      while(IsFull()){
        WakeUpComsumer();
        ProductorWait();//如果这个函数调用失败呢，那么该线程没有挂起
        //因此为了程序的健壮性需要将if判断语句改为while循环，进行不断的检测IsFull()
      }

      q.push(t);
      UnLockQueue();
    }

    void Get(Task &t)
    {
      LockQueue();
      while(IsEmpty()){
        WakeUpProductor();
        ComsumerWait();
      }
      t=q.front();
      q.pop();
      UnLockQueue();
    }
       
    ~BlockQueue()
    {
      pthread_mutex_destroy(&lock);
      pthread_cond_destroy(&c_cond);
      pthread_cond_destroy(&p_cond);
    }
};
#endif
