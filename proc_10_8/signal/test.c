#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<wait.h>

void handler(int sig){
  printf("pid:%lu,get a sig ,No.%d\n",getpid(),sig);
  int status=0;
 int ret= wait(&status);
 printf("ret:%d\n",ret);
 if(ret>0){
   printf("child exit code:%d\n",(status>>8)&&0XFF);
 } 
}

int main(){
  signal(SIGCHLD,handler);
 //signal(SIGCHLD,SIG_IGN);//忽略该信号

  if(fork()==0){
    printf("child running ...m:pid:%lu,ppid:%lu\n",getpid(),getppid());
    sleep(5);
    printf("child quit!\n");
    exit(1);
  }
  while(1);
  return 0;

}
