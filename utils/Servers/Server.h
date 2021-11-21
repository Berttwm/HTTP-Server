#ifndef Server_H
#define Server_H

#include "../Lib-sockets.h"
#include "../Lib-threads.h"
#include <stdio.h>
#include <unistd.h>


class Server
{
private:
    /* Fixed Server Config */
    int HTTP_Port = 12346;
    int backlog = 10000;
    ListeningSocket *socket;
    virtual void acceptor() = 0;
    virtual void handler() = 0;
    virtual void responder() = 0;
public:
    Server(int domain, int service, int protocol, int port, u_long interface, int bklog);

    virtual void launch() = 0;
    // Getter
    ListeningSocket *get_socket();
};

#endif //Server_H