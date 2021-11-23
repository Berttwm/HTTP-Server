#ifndef HTTPServer_H
#define HTTPServer_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>


#include "Server.h"
#include "HTTP_Tools/HTTP_Request.h"
#include "HTTP_Tools/HTTP_Response.h"

class HTTPServer : public Server
{
private:
    /* Thread Pool */
    ThreadPool TP;

    /* Fixed file director for polling HTML objects */
    const std::string m_directory = "./server_resources";
    
    /* Fixed HTTP Server Config */
    int HTTP_Port = 12346;
    int backlog = 10000;

    /* Strings for special conditions (HTTP 1.1) */
    const std::string Keep_Alive_str = "Keep-Alive";
    const std::string Connection_str = "Connection";
    
    char buffer[30000];
    void handler(int &arg);

    void enable_keepalive(int sock, int seconds_alive);
public:
    HTTPServer();
    void launch();

};

#endif // HTTPServer_H