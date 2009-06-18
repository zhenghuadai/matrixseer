/*
 * =====================================================================================
 *
 *       Filename:  testpop3.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年08月10日 11时18分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */

#include "pop3socket.cpp"

int main(int argc,char** argv)
{
    int ret;
    int sockfd;
    int errcode =    pop3Connect("pop3.sina.com.cn","djx_zh","djx_zh010",sockfd);
    printf("errcode :%d\n",errcode);
    int len1 = pop3List(sockfd,2);
    printf("%d\n",len1);
    printf("testList\n");
    buf_t buf;
    //pop3List(sockfd,buf);
    printf("testUrdl\n");
    pop3Uidl(sockfd,1,(buf.buf));
    //printf("%s\n",buf.buf);
    printf("testRetr\n");
    pop3Retr(sockfd,22,buf);
    //int mails=0,len=0;
    //pop3Stat(sockfd,mails,len);
    //printf("%d %d \n",mails,len);
    //ret =     pop3Top(sockfd,30,10,buf);
    //if(ret == SOCKSUCESS)
    //printf("%d\n",buf.len);
    //buf.print();
}
