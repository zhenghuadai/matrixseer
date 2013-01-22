/*
 * =====================================================================================
 *
 *       Filename:  .h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/15/2010 08:24:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  DMSOCKET_INC
#define  DMSOCKET_INC

#include <sys/socket.h>

class DmSocket{

    public:
        DmSocket(){};
        ~DmSocket(){};

    public:
        ssize_t sendto (__const void *__buf, size_t __n,
                int __flags, __CONST_SOCKADDR_ARG __addr,
                socklen_t __addr_len){ ::sendto(m_socket, __buf, __n, __flags, __addr, __addr_len);};
        ssize_t recvfrom (void *__restrict __buf, size_t __n,
                int __flags, __SOCKADDR_ARG __addr,
                socklen_t *__restrict __addr_len){::recvfrom(m_socket, __buf, __n, __flags, __addr, __addr_len);}

    protected:
        int m_socket;
};
#endif   /* ----- #ifndef DMSOCKET_INC  ----- */
