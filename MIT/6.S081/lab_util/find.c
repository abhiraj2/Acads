#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void strcat(char* pd, char* p1, char* p2){
    while(*p1){
        *pd++ = *p1++;
    }
    while(*p2){
        *pd++ = *p2++;
    }
    *pd = '\0';
}

void find(char* path, char* file){
    int fd;
    struct stat fdStat;
    struct dirent dir;
    char buf[512];

    //printf("%s\n\n", path);
    fd = open(path, 0);
    if(fstat(fd, &fdStat) < 0){
        printf("Error\n");
        exit(1);
    }
    if(fdStat.type == T_FILE){
        printf("error madi\n");
        exit(1);
    }
    else if(fdStat.type == T_DIR){
        int templen = strlen(path);
        path[templen] = '/';
        path[templen+1] = '\0';
        while(read(fd, &dir, sizeof(dir)) > 0){
            strcpy(buf, dir.name);
            if(strlen(buf) <1) continue;
            struct stat tempStat;
            char fullPath[512];
            strcat(fullPath, path, buf);
            stat(fullPath, &tempStat);
            //printf("%s\n %d\n", fullPath, strlen(fullPath));
            if(tempStat.type == T_FILE){
                if(!strcmp(buf, file)){
                    printf("%s\n", fullPath);

                }
            }
            else if(tempStat.type == T_DIR){
                if(strcmp(buf, ".") != 0 && strcmp(buf, "..") != 0){
                    //printf("%s\n %d\n", fullPath, strlen(fullPath));
                    find(fullPath, file);
                }
            }
        }
    }
    close(fd);
}

int main(int argc, char* argv[]){
    // char path[512];
    // strcat(path, argv[1], "/");
    find(argv[1], argv[2]);
    exit(0);
}