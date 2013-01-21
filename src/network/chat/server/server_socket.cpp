#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include "server_socket.h"
void Server_socket::init()
{
    int ret;
    socklen_t server_len=16;
    struct sockaddr_in server_addr;
    int i;
    if((server=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket err\n");
        exit(0);
    }
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(0);
    server_addr.sin_port=htons(SERVER_PORT);
    if(bind(server,(struct sockaddr*)&server_addr,16) !=0)
    {
        perror("bind err\n");
        exit(0);
    }
    if((ret = getsockname(server,(struct sockaddr *)&server_addr,&server_len)) !=0)
    {
        printf("getsockname err %d\n", ret);
        exit(0);
    }
    printf("server use port%d\n",ntohs(server_addr.sin_port));
}
