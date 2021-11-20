#include "socket.h"

Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
    // define address structure
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);

    // establish socket
    sock = socket(domain, service, protocol);
    test_connection(sock);

    // establish network
    // connection = connect_to_network(sock, address);
    // test_connection(connection);
}

void Socket::test_connection(int item_to_test)
{
    if(item_to_test < 0)
    {
        perror("[Error] Failed to connect...");
        exit(EXIT_FAILURE);
    }
}
struct sockaddr_in Socket::get_address()
{
    return address;
}

int Socket::get_sock()
{
    return sock;
}

int Socket::get_connection()
{
    return connection;
}