#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void prime(int p1[2]){
    int num=0;
    int first_num=0;
    wait(0);
    sleep(1);
    read( p1[0],&num, 4);
    if (num==0){
        exit(0);
    }
    first_num=num;
    printf("prime %d\n", first_num);
    sleep(1);
    int p2[2];
    pipe(p2);
    int pid=fork();
    if (pid > 0){
        close(p2[0]);
        while (num!=0){
            read( p1[0],&num, 4);
            if (num%first_num!=0){
                sleep(1);
                write(p2[1],&num,4);
            }
            sleep(1);
        }
        close(p2[1]);
        close(p1[0]);
        close(p1[1]);
    }else{
        sleep(1);
        close(p1[1]);
        prime(p2);
        close(p1[0]);
        
        exit(0);
    }
    
}


int main(){
    int p[2];
    pipe(p);
    int pid= fork();
    if(pid > 0) {
        close(p[0]);
        for(int i=2;i<=35;i++){
            write(p[1],&i,4);
            sleep(2);
        }
        close(p[1]);
    }
    else{
        prime(p);
        close(p[0]);
        close(p[1]);
    }
    sleep(2);
    exit(0);
}