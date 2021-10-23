#include"udpServer.hpp"

void Usage(std::string proc){
  std::cout<<"Usage: "<<proc<<" local_port"<<std::endl;
}

//写一个可以进行任意IP与端口号绑定的服务器
int main(int argc,char* argv[]){
  if(argc!=3){//0号下标默认对应可执行程序的名称,因此argv[0]不为空
    Usage(argv[0]);//使用手册
    exit(1);
  }
  udpServer *up=new udpServer(argv[1],atoi(argv[2]));
  up->initServer();
  up->start();
  return 0;
}
