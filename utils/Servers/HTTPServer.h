#ifndef HTTPServer_H
#define HTTPServer_H

#include <stdio.h>
#include <string.h>

#include "Server.h"
class HTTPServer : public Server
{
private:
    /* Thread Pool */
    ThreadPool TP;
    
    /* Initialized HTTP config */
    int port = 12346;
    int backlog = 100000;

    char buffer[30000];
    int new_socket;
    void acceptor();
    void handler();
    void responder();
public:
    HTTPServer();
    void launch();
};

#endif // HTTPServer_H