#include"Client.hpp"

void Usage(std::string proc){
  std::cout<<"Usage: \n"<<"\t";
  std::cout<<proc<<" svr_ip svr_port"<<std::endl;
}

int main(int argc,char *argv[]){
  if(argc!=3){
    Usage(argv[0]);
    exit(1);
  }
  client *tc=new client(argv[1],atoi(argv[2]));

  tc->initClient();
  tc->start();

  delete tc;
  return 0;
}
