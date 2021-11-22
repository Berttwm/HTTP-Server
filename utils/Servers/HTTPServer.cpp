#include "HTTPServer.h"

void HTTPServer::handler(int &arg)
{
    int thread_sock = arg;
    char request_string[30000];
    read(thread_sock, request_string, 30000); // 30000 is the request_str_len

    std::cout << request_string << std::endl;
    HTTP_Request *HReq = new HTTP_Request(std::string(request_string));

    // HTTP 1.1 only handles GET method at the moment
    std::string msg;
    if(HReq->get_method() != Method::GET) {
         msg = "HTTP/1.1 405 Method not Allowed\r\nAllow: GET\r\n\r\n";
    } else {
        // Successful HTTP/1.1 GET request found
        HTTP_Response *HResp = new HTTP_Response(HTTPServer::m_directory);
        msg = HResp->write_responses(*HReq);
        msg = "Server successful ...";

    }
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
        // handler methods - to abstract into threads
        TP.add_job(std::bind(&HTTPServer::handler, this, new_socket));
        std::cout << "==== HTTP Server Done: ====" << std::endl;
    }
}
