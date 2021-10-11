#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;

void*routine_r(void*arg){
  const char*name=(char*)arg;
  while(1){
    pthread_cond_wait(&cond,&lock);
    printf("get cond,%s 活动...\n",name);
  }
}

void*routine_r1(void*arg){
  const char*name=(char*)arg;

  while(1){
    sleep(rand()%3+1);
    pthread_cond_signal(&cond);
    printf("%s signal done ...\n",name);
  }
}
int main(){
  pthread_mutex_init(&lock,NULL);
  pthread_cond_init(&cond,NULL);

  pthread_t t1,t2,t3,t4,t5;
  pthread_create(&t1,NULL,routine_r1,"thread1");
  pthread_create(&t2,NULL,routine_r,"thread2");
  pthread_create(&t3,NULL,routine_r,"thread3");
  pthread_create(&t4,NULL,routine_r,"thread4");
  pthread_create(&t5,NULL,routine_r,"thread5");

  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  pthread_join(t3,NULL);
  pthread_join(t4,NULL);
  pthread_join(t5,NULL);

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond);
  return 0;
}
