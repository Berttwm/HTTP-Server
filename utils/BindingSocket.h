#ifndef BindingSocket_H
#define BindingSocket_H

#include <stdio.h>

#include "Socket.h"

class BindingSocket: public Socket
{
    BindingSocket(int domain, int service, int protocol, int port, u_long interface);

    int connect_to_network(int sock, struct sockaddr_in address);
};

#endif // BindingSocket_H