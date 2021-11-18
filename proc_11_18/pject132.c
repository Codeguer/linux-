#include<stdio.h>

struct S{
    char c1;
    int i;
    char c2;
};

int main(){
    printf("%d\n",sizeof(struct S));
    return 0;
}
