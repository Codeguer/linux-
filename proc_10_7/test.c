#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void show_pending(sigset_t *pending){
  int sig=1;
  for(;sig<=31;sig++){
    if(sigismember(pending,sig)){
      printf("1");
    }
    else{
      printf("0");
    }
  }
  printf("\n");
}

void handler(int sig){
  printf("get a sig : %d\n",sig);
}

int main(){
  struct sigaction act,oact;

  act.sa_handler=handler;
  act.sa_flags=0;
  sigemptyset(&act.sa_mask);

  sigaction(2,&act,&oact);

  while(1);
    




//  //捕捉2号信号
//  signal(2,handler);
//  //定义sigset_t类型的变量，该局部变量定义在用户的栈上,而不是进程中
//  sigset_t pending;
//
//  sigset_t block,oblock;
//  sigemptyset(&block);
//  sigemptyset(&oblock);
//
//  //设置后再通过sigprocmask函数可以将2号信号屏蔽
//  sigaddset(&block,2);//这个函数只是对用户自己定义的变量进行修改
//
//  //调用函数sigprocmask才可以读取或更改进程的信号屏蔽字
//  sigprocmask(SIG_SETMASK,&block,&oblock);//将进程中的block先备份到oblock，再设置为传入的block
//
//  int count=0;
//  while(1){
//    sigemptyset(&pending);//对用户定义的变量进行清空
//    sigpending(&pending);/*将进程中的pending未决信号集（因为2号信号被屏蔽了因此2号信号如果有的话在这里一定是未决信号）
//    通过用户定义的pending变量传出*/
//    
//    show_pending(&pending);
//    sleep(1);
//    count++;
//    if(count==20){
//      printf("recover sig mask\n");
//      sigprocmask(SIG_SETMASK,&oblock,NULL);//恢复进程中的block到一开始的状态
//    }
//  }

  return 0;
}
