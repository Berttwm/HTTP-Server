#ifndef HTTPServer_H
#define HTTPServer_H

#include <stdio.h>
#include <string.h>

#include "Server.h"
#include "HTTP_Tools/HTTP_Request.h"


class HTTPServer : public Server
{
private:
    /* Thread Pool */
    ThreadPool TP;
    
    /* Fixed HTTP Server Config */
    int HTTP_Port = 12346;
    int backlog = 10000;

    char buffer[30000];
    void handler(int &arg);
public:
    HTTPServer();
    void launch();

};

#endif // HTTPServer_H