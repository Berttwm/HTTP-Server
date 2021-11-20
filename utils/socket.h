#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class Socket
{
private:
    struct sockaddr_in address;
    int sock;
    int connection;
public:
    Socket(int domain, int service, int protocol, int port, u_long interface);
    
    virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;

    void test_connection(int item_to_test);
    struct sockaddr_in get_address();
    int get_sock();
    int get_connection();
};
#endif // SOCKET_H