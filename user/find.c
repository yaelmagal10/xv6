#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p)+1);
  return buf;
}

void find(char *name, char *path){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, O_RDONLY)) < 0){
        return;
    }
    if(fstat(fd, &st) < 0){
        close(fd);
        return;
    }
    switch(st.type){
        case T_FILE:
        if (strcmp(fmtname(path),name)==0){
            printf("%s\n",path);
        }
        break;
        case T_DIR:
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (strcmp(".",fmtname(buf))==0)
                continue;
            else if (strcmp("..",fmtname(buf))==0)
                continue;
            else if(stat(buf, &st) >= 0){
                find(name,buf);
            } 
        }
        break;
    }
    close(fd);
}


int main(int argc, char *argv[]){
    if (argc<2){
        exit(1);
    }
    char *depth=argv[1];
    char *file_name=argv[2];
    find(file_name,depth);
    exit(0);
}