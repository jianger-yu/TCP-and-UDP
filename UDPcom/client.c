#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
char buf[BUFSIZ];

struct sockaddr_in addr;
int clit_addr_len = 0;

void sys_err(char * s){
    perror(s);
    exit(1);
}

int main(){
    int fd = 0, ret = 0;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
        sys_err("socket error");
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1145);
    ret = inet_pton( AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    if(ret < 0)
        sys_err("inet_pton error");
    else if (ret == 0) {
        fprintf(stderr, "Invalid IP address format\n");
        exit(1);
    }
    
    while(1){
        memset(buf,0,sizeof buf);
        read(STDIN_FILENO, buf, sizeof buf);
        ret = sendto(fd,buf,sizeof buf,0,(struct sockaddr *)&addr, sizeof addr);
        if(ret == -1) sys_err("sendto error");

        ret = recvfrom((int)fd,buf,sizeof(buf),0,NULL,0);
            if(ret == -1) sys_err("recvfrom error");
        write(STDOUT_FILENO,buf,ret);
    }
    close(fd);
    return 0;
}