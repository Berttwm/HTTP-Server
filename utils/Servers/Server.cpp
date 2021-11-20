#include "Server.h"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int bklog)
{
    socket = new ListeningSocket(domain, service, protocol, port, interface, bklog);
}

ListeningSocket *Server::get_socket()
{
    return socket;
}