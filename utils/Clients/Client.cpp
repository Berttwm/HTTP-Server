#include "Client.h"

Client::Client(int domain, int service, int protocol, int port, u_long interface)
{
    socket = new ConnectingSocket(domain, service, protocol, port, interface);
}

ConnectingSocket *Client::get_socket()
{
    return socket;
}
