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

#ifndef  CHAT_SERVER_INC
#define  CHAT_SERVER_INC
#include "server_socket.h" 
class Chat_server{

    public:
        Chat_server(){};
        ~Chat_server(){};

    public:
        void init();
        int processMsg(char *buf,struct sockaddr_in addr);
        void run();


    private:
        Server_socket serverSocket;
};
#endif   /* ----- #ifndef server_socket_INC  ----- */
