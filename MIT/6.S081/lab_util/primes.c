#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int p[2]){
    close(p[1]);
    int r;
    read(p[0], &r, 4);
    if(r>35) return;
    printf("prime %d\n", r);
    int newPipe = 0;
    int p1[2];
    int status = pipe(p1);
    if(status < 0){
        printf("error\n%d\n", status);
        exit(1);
    }
    
    while(1){
        int temp;
        read(p[0], &temp, 4);
        //printf("%d ", temp);
        if(temp>35) break;
        
        if(temp%r){
            if(!newPipe && fork()==0){
                prime(p1);   
            }
            else{
                newPipe = 1;
                close(p1[0]);
                write(p1[1], &temp, 4);
                sleep(1);
            }
        }
        else continue;
    }
}

int main(){
    int p1[2];
    pipe(p1);
    int pid = fork();
    if(pid == 0){
        prime(p1);
    }
    else{
        close(p1[0]);
        for(int i=2; i<=35; i++){
            write(p1[1], &i, 4);
            sleep(1);
        }
    }
    exit(0);
}
