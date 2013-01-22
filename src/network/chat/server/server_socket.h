/*
 * =====================================================================================
 *
 *       Filename:  server_socket.h
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

#ifndef  SERVER_SOCKET_INC
#define  SERVER_SOCKET_INC
#include <sys/socket.h>
#include "dmsocket.h"
#define DZH "DZH"
#define NC "NCC"
#define DZHTH 1
#define NCTH 0
#define MAXES 10
#define LOGOFF 8
#define SERVER_PORT 1236
class Server_socket:public DmSocket
{

    public:
        Server_socket(){};
        ~Server_socket(){};

    public:
        void init();
        void active();
};
#endif   /* ----- #ifndef server_socket_INC  ----- */
