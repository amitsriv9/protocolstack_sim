#include <stdio.h>
#include <stdlib.h>

  int main(){
    unsigned short  x=0x0;
    unsigned short  mask1=0x8000,mask2=0x0FFF;
    printf("x is %d\n",x);
    x=mask1;
    printf("x is %X\n",x);
    x = x|mask2;
    printf("x is %X\n",x);
  return 0;
  }
