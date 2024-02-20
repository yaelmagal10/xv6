#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(){
    int p[2];
    char * buf= "";
    pipe(p);
    int pid= fork();
    if(pid == 0) {
        read( p[0],buf, 1);

        printf("%d: received ping\n",getpid());
        
        close(p[0]);
        write(p[1],buf,1);

        close(p[1]);
    } else {
        close(p[0]);

        write(p[1],"1",1);

        wait(0);
        read( p[0],buf, 1);

        printf("%d: received pong\n",getpid());

        close(p[0]);
        close(p[1]);
    }
    exit(0);
}
