#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define FIFO_FILE "./fifo"
int main(){
  int fd=open(FIFO_FILE,O_WRONLY);
  if(fd>0){
    char buf[64];
    while(1){
      printf("Please Enter Message # ");
      fflush(stdout);//要刷新缓冲区，不然printf打印的内容就在缓冲区我们就看不到了
      ssize_t s=read(0,buf,sizeof(buf)-1);
      if(s>0){
        buf[s]=0;
        write(fd,buf,s);
      }
    }
  }
}
