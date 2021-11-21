#include "HTTPServer.h"

void HTTPServer::acceptor()
{
    Socket *curr_socket = this->get_socket();
    struct sockaddr_in address = curr_socket->get_address();
    int addrlen  = sizeof(address);
    new_socket = accept(curr_socket->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    
    if(new_socket < 0) 
    {
        std::cout << "new_socket = (" << new_socket << ")" << std::endl;
        perror("in accept");
        exit(EXIT_FAILURE);
    }

    read(new_socket, buffer, 30000); // 30000 is the buffer_len
}
void HTTPServer::handler()
{
    std::cout << buffer << std::endl;
}
void HTTPServer::responder()
{
    std::string msg = "Server response ...";
    write(new_socket, msg.c_str(), msg.length());
    close(new_socket);
}

HTTPServer::HTTPServer()
: Server(AF_INET, SOCK_STREAM, 0, 12346, INADDR_ANY, 10) // port 80 for HTTP
{
    get_socket()->start_listening();
    launch();
}

void HTTPServer::launch()
{
    while(true)
    {
        std::cout << "==== HTTP Server Waiting: ====" << std::endl;
        acceptor();
        handler();
        responder();
        std::cout << "==== HTTP Server Done: ====" << std::endl;
    }
}


