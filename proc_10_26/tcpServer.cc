#include"tcpServer.hpp"

void Usage(std::string proc){
  std::cout<<"Usage: "<<std::endl;
  std::cout<<'\t'<<proc<<" port"<<std::endl;
}

int main(int argc,char* argv[]){
  if(argc!=2){
    Usage(argv[0]);
    exit(1);
  }

  tcpServer *tp=new tcpServer(atoi(argv[1]));
  tp->initServer();
  tp->start();

  delete tp;
  
  return 0;
}
