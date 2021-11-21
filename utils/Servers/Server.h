#ifndef Server_H
#define Server_H

#include "../Lib-sockets.h"
#include "../Lib-threads.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>


class Server
{
private:
    ListeningSocket *socket;
    virtual void handler(int &arg) = 0;
public:
    Server(int domain, int service, int protocol, int port, u_long interface, int bklog);

    virtual void launch() = 0;
    // Getter
    ListeningSocket *get_socket();
};

#endif //Server_H