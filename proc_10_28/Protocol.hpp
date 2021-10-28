#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include<iostream>

typedef struct request{
  int x;//操作数1,left
  int y;//操作数2,right
  char op;//运算符,+-*/%
}request_t;

typedef struct response{
  int code;//当code为0表示此时的数据正常可以进行计算，为1、2、3....表示相应的错误
  //这里写的代码是基于应用层的自定义协议代码
  int result;
}response_t;
#endif
