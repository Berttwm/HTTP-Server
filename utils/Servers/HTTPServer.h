#ifndef HTTPServer_H
#define HTTPServer_H

#include <stdio.h>
#include <string.h>

#include "Server.h"
class HTTPServer : public Server
{
private:
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