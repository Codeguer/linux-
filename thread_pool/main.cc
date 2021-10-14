#include"ThreadPool.hpp"

int main(){
  ThreadPool *tp=new ThreadPool();
  tp->ThreadPoolInit();

  //server
  int count=20;//20个任务量
  while(count){
    int x=rand()%10+1;
    Task t(x);
    tp->Put(t);
    sleep(1);
    count--;
  }

  tp->ThreadQuit();//这种方法在线程池当中只是为了让线程池的代码体现出完整性
  //实际上作为一个服务器所有的业务线程只会减少但不会永远消失
  return 0;
}
