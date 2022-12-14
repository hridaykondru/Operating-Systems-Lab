#include<stdio.h>
int main(){
    char in;
    int count=0;
    while((in=getchar())!=EOF){
        if(!(in>='a' && in<='z') && !(in>='A' && in<='Z') && (in!='\n') && (in!=' ')){
            ++count;
        }
        putchar(in);
    }
    fprintf(stderr,"final count = %d\n",count);
    //fflush(stdout);
    return(0);
}
