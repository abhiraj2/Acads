#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int p[2];
    char c = 'b';
    pipe(p);
    int pid; 
    pid = fork();
    if(pid == 0){
        write(p[1], &c, 1);
        printf("%d: received ping\n", getpid());
    }
    else{
        wait(0);
        char r;       
        read(p[0], &r, 1 );
        //printf("%c\n", r);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}