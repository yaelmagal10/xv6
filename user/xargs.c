#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"




int main(int argc, char *argv[]){
    int n=1;
    int fd = 0;
    char buff[1];
    while (1){
        char names[32][32];
        for (int i=0;i<32;i++){
            for (int j=0;j<32;j++){
                names[i][j]='\0';
            }
        }
        int index=0;
        int index_name=0;
        int end_index=0;
        while (1){
            n=read(fd, buff, sizeof(buff));
            if ((n==0)||(*buff=='\n')){
                break;
            }
            if (*buff==' '){
                names[index][index_name]='\0';
                index++;
                index_name=0;
            }else{
                names[index][index_name]=*buff;
                index_name++;
                if (index_name==1){
                    end_index++;
                }
            }
        }
        if (n==0){
            break;
        }
        int pid=fork();
        if(pid > 0) {
            wait(0);
        }else{
            char * args[end_index+argc-1];
            for (int i=1;i<argc;i++){
                args[i-1]=argv[i];
            }
            for (int i=0;i<end_index;i++){
                args[argc-1+i]=names[i];
            }
            //printf("%d %s %s %s\n",end_index+argc-1,args[0],args[1],args[2]);
            exec(argv[1],args);
            wait(0);
        }
    }
    exit(0);
}