#include<stdio.h>
int main(){
    char in;
    while((in=getchar())!=EOF){
        if(in>='a' && in<='z'){
           putchar(in-32);
        }
        else if(in>='A' && in<='Z'){
            putchar(in+32);
        }
        else{
            putchar(in);
        }
    }
    
    return(0);
}
