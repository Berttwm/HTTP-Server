#include "HTTPServer.h"

void HTTPServer::handler(int &arg)
{
    int thread_sock = arg;
    char request_string[30000];
    read(thread_sock, request_string, 30000); // 30000 is the request_str_len

    std::cout << request_string << std::endl;

    std::string msg = "Server response ...";
    write(thread_sock, msg.c_str(), msg.length());
    close(thread_sock);
}

HTTPServer::HTTPServer()
: Server(AF_INET, SOCK_STREAM, 0, 12346, INADDR_ANY, 10000) // port 80 for HTTP
{
    // initiate thread pool:
    get_socket()->start_listening();
    launch();
}

void HTTPServer::launch()
{
    while(true)
    {
        std::cout << "==== HTTP Server Waiting: ====" << std::endl;
        // acceptor method
        int new_socket;
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
        // handler methods
        TP.add_job(std::bind(&HTTPServer::handler, this, new_socket));
        std::cout << "==== HTTP Server Done: ====" << std::endl;
    }
}
