#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>

void * thread_run(void *arg){
  while(1){
    printf("I am %s,pid:%d \n",(char*)arg,getpid());
    sleep(1);
  }
}

int main(){
  pthread_t tid;
  pthread_create(&tid,NULL,thread_run,(void*)"thread 1");//创建新线程
  //新线程执行thread_run函数
  
  //主线程创建新线程后继续往下执行
  while(1){
    printf("I am main thread!,pid:%d\n",getpid());
    sleep(2);
  }
}
