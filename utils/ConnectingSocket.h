#ifndef ConnectingSocket_H
#define ConnectingSocket_H

#include "Socket.h"

class ConnectingSocket: public Socket
{
    ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);

    int connect_to_network(int sock, struct sockaddr_in address);
};


#endif // ConnectingSocket_H