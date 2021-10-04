#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include"comm.h"
#include<sys/shm.h>

int main(){
  key_t k=ftok(PATHNAME,PROJ_ID);
  printf("key:%#X\n",k);

  int shmid=shmget(k,SIZE,0);//给0默认获取已经创建好的共享内存（通过k值查找）
  if(shmid<0){
    perror("shmget");
    return 1;
  }
  printf("shmid:%d\n",shmid);

  char*str=(char*) shmat(shmid,NULL,0);

  char c='a';
  for(;c<='z';c++){
    str[c-'a']=c;
    sleep(5);
  } 

  shmdt(str);

  sleep(3);

  return 0;
}
