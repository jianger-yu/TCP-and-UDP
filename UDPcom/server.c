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

struct sockaddr_in addr , caddr;
int clit_addr_len = 0;
void sys_err(char * s){
    perror(s);
    exit(1);
}


int main(){
    int fd = 0, ret = 0, cfd;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
        sys_err("socket error");
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1145);              //转移port
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //转移ip

    bind(fd ,(struct sockaddr*)&addr, sizeof(addr));
    
    while(1){
        memset(buf,0,sizeof buf);
        clit_addr_len = sizeof addr;
        int ret = recvfrom((int)fd,buf,sizeof(buf),0,(struct sockaddr *)&caddr, &clit_addr_len);
        if(ret == -1) sys_err("recvfrom error");

        write( STDOUT_FILENO, buf, ret);        
        ret = sendto(fd,buf,ret,0,(struct sockaddr *)&caddr, sizeof caddr);
        if(ret == -1) sys_err("sendto error");
    }
    close(fd);
    //close(cfd);
    return 0;
}