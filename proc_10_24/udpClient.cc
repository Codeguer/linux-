#include"udpClient.hpp"

void Usage(std::string proc){
  std::cout<<"Usage: "<<proc<<" svr_ip svr_port"<<std::endl;
}
int main(int argc,char *argv[]){
  if(argc!=3){
    Usage(argv[0]);
    exit(1);
  }
  //CV模式，即客户端永远是主动的，服务器是被动的
  //因此客户端先访问服务器，那么就需要知道服务器的IP与port
  udpClient uc(argv[1],atoi(argv[2]));
  uc.initClient();
  uc.start();
  return 0;
}
