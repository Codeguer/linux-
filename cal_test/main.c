#include"cal.h"

int main()
{
  int x,y;
  while(1)
  {
    printf("Please# ");
    scanf("%d %d",&x,&y);
    int rt=MyAdd(x,y);
    printf("%d+%d=%d\n",x,y,rt);
  }
}
