#include "EpollServer.hpp"

int main(){
  EpollServer *es=new EpollServer(8080);
  es->InitServer();
  es->Start();

  return 0;
}
