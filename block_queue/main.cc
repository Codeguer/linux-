#include"BlockQueue.hpp"

using namespace std;
pthread_mutex_t p_lock;
pthread_mutex_t c_lock;

void*consumer_run(void*arg){
  BlockQueue *bq=(BlockQueue*)arg;
  while(true){
    Task t;
    pthread_mutex_lock(&c_lock);
    bq->Get(t);
    cout<<"consumer: "<<t.x<<"+"<<t.y<<"="<<t.Run()<<endl;
    pthread_mutex_unlock(&c_lock);
  }
}

void*productor_run(void*arg){
  sleep(1);
  BlockQueue *bq=(BlockQueue*)arg;
  while(true){
    int x=rand()%10+1;
    int y=rand()%100+1;

    pthread_mutex_lock(&p_lock);
    Task t(x,y);
    bq->Put(t);
    cout<<"product Task  is :"<<x<<"+"<<y<<"= ?"<<endl;
    sleep(1);
    pthread_mutex_unlock(&p_lock);
  }
}

int main(){
  BlockQueue*bq=new BlockQueue(5);
  pthread_t c,c1,c2,c3,c4,p,p1,p2,p3,p4;
  pthread_create(&c,nullptr,consumer_run,(void*)bq);
  pthread_create(&c1,nullptr,consumer_run,(void*)bq);
  pthread_create(&c2,nullptr,consumer_run,(void*)bq);
  pthread_create(&c3,nullptr,consumer_run,(void*)bq);
  pthread_create(&c4,nullptr,consumer_run,(void*)bq);
  pthread_create(&p,nullptr,productor_run,(void*)bq);
  pthread_create(&p1,nullptr,productor_run,(void*)bq);
  pthread_create(&p2,nullptr,productor_run,(void*)bq);
  pthread_create(&p3,nullptr,productor_run,(void*)bq);
  pthread_create(&p4,nullptr,productor_run,(void*)bq);

  pthread_join(c,nullptr);
  pthread_join(c1,nullptr);
  pthread_join(c2,nullptr);
  pthread_join(c3,nullptr);
  pthread_join(c4,nullptr);
  pthread_join(p,nullptr);
  pthread_join(p1,nullptr);
  pthread_join(p2,nullptr);
  pthread_join(p3,nullptr);
  pthread_join(p4,nullptr);

  delete bq;
  return 0;
}
