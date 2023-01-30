#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    char *(new_argv)[16];

    for(int j=0; j< 16; j++){
        new_argv[j] = (char*)malloc(sizeof(char)*128);
    }

    char from_stdin[128];
    int i=0;
    //int j=0; 
    while(read(0, from_stdin+i, 1) > 0){
        i++;
    }

    i=0;
    for(int j=1; j<argc; j++){
        strcpy(new_argv[i++], argv[j]);
    }

    char* trav1 = from_stdin;
    char* trav2 = new_argv[i];
    while(*trav1){
        if( *trav1 == '\n' || (*trav1 == '\\' && *(trav1+1)=='n')){
            *(trav2++) = '\0';
            trav2 = new_argv[++i];
            trav1++;
            trav1++;
        }
        else{
            *(trav2) = *(trav1) ;
            trav1++;
            trav2++;
        }
    }
    new_argv[i+1] = 0;
    //int new_argc = i;
    // for(int j=0; j<new_argc; j++){
    //     printf("%s\n", new_argv[j]);
    // }
    
    exec(new_argv[0], new_argv);

    //printf("%s\n", new_argv[1]);
    //printf("%s\n", trav2);
    exit(0);
}