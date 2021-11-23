#ifndef Client_H
#define Client_H

#include "../Lib-sockets.h"
#include "../Lib-threads.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>


class Client {
private:
    ConnectingSocket *socket;
public:
    Client(int domain, int service, int protocol, int port, u_long interface);

    virtual void launch() = 0;
    // Getter
    ConnectingSocket *get_socket();
};

#endif // Client_H