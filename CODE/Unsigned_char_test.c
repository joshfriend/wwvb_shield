#include <stdio.h>

int main(void){
    unsigned char a = 1;
    unsigned char b = 4;
    unsigned char c = a+b;
    
    while(1){
             printf("%u + %u = %u \n",a,b,c);
             printf("%c + %c = %c \n",a,b,c);
    }

    return 0;
}
