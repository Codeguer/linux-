#include<stdio.h>
#include<stdlib.h>

int compare(const void *p1, const void *p2){
    return *(int*)p1-*(int*)p2;
}

void Swap(void *const p1,void *const p2,int size){
    for(int i=0;i<size;i++){
        char temp=*((char*)p1+i);
        *((char*)p1+i)=*((char*)p2+i);
        *((char*)p2+i)=temp;
    }
}
void MyQsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)){
    //使用冒泡排序
    for(int i=0;i<nmemb-1;i++){
        int flag=1;
        for(int j=1;j<nmemb-i;j++){
            if(compar((char*)base+size*(j-1),(char*)base+size*j)>0){
                Swap((char*)base+size*(j-1),(char*)base+size*j,size); 
                flag=0;
            }
        }
        if(flag){
            return;
        }
    }
}
int main(){
    int arr[]={44,25,67,323,76,34,63,2,734,24,343}; 
    int sz=sizeof(arr)/sizeof(arr[0]);
   // qsort(arr,sz,sizeof(arr[0]),compare);
    MyQsort(arr,sz,sizeof(arr[0]),compare);
    for(int i=0;i<sz;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
    return 0;
}
