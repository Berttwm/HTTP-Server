#include "HTTPServer.h"

/* 
* Handler to pass connection jobs off to other threads within thread pool
* HTTP 1.1 (TODO, components commented out due to correctness issue)
    1) Get persistent connection with synchronized read(svr) & write(client)
    2) Extract "keep-alive" HTTP field and do necessary socket processing
* HTTP 1.0 (Completed): 
    1) Read request
    2) Parse HTTP Request (GET only for now) and extract important info
    3) Respond back with appropriate message.
*/
void HTTPServer::handler(int &arg)
{
    int thread_sock = arg;
    // char request_string[30000];
    // while(true)
    // {
        char request_string[30000];
        if(read(thread_sock, request_string, 30000) < 0)
        {
            // continue;
        }

        std::cout << "Server has received a message!" << std::endl;
        HTTP_Request *HReq = new HTTP_Request(std::string(request_string));
        // // detect for keep-alive connections
        std::string connection_header = HReq->get_header(this->Connection_str);
        std::string keep_alive_header = HReq->get_header(this->Keep_Alive_str);
        if(connection_header == "keep-alive" && keep_alive_header != "") {
            int keep_alive_duration = stoi(keep_alive_header);
            this->enable_keepalive(thread_sock, keep_alive_duration);
            // std::cout << "keep-alive enabled for: " << keep_alive_duration << "s" << std::endl;
        }
        // // HTTP 1.1 only handles GET method at the moment
        std::string msg;
        if(HReq->get_method() != Method::GET) {
            msg = "HTTP/1.1 405 Method not Allowed\r\nAllow: GET\r\n\r\n";
        } else {
            // Successful HTTP/1.1 GET request found
            HTTP_Response *HResp = new HTTP_Response(HTTPServer::m_directory);
            msg = HResp->write_responses(*HReq);
            // msg = "Server successful ...";
         }
        if(write(thread_sock, msg.c_str(), msg.length()) < 0){

        }
    // }
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
        std::cout << "==== HTTP Server Waiting on port:" << HTTP_Port << "====" << std::endl;
        // acceptor method
        int new_socket;
        Socket *curr_socket = this->get_socket();
        struct sockaddr_in address = curr_socket->get_address();
        int addrlen  = sizeof(address);
        new_socket = accept(curr_socket->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if(new_socket < 0) 
        {
            perror("in accept");
            exit(EXIT_FAILURE);
        }
        // handler methods - to abstract into threads
        TP.add_job(std::bind(&HTTPServer::handler, this, new_socket));
        std::cout << "==== HTTP Server Done: ====" << std::endl;
    }
}

// For keepalive connections
void HTTPServer::enable_keepalive(int sock, int seconds_alive)
{
    int yes = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) < 0){
        perror("TCP_keepalive failed");
        exit(EXIT_FAILURE);
    }

    /* For some reason my distro cannot detect TCP_KEEPIDLE value and I am unable to set this TCP field */
    // if(setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &seconds_alive, sizeof(int)) != -1){
    //     perror("TCP_KEEPIDLE failed");
    //     exit(EXIT_FAILURE);
    // }

    // int interval = 1;
    // if(setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(int)) != -1){
    //     perror("TCP_KEEPINTVL failed");
    //     exit(EXIT_FAILURE);
    // }

    // int maxpkt = 10;
    // if(setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &maxpkt, sizeof(int)) != -1){
    //     perror("TCP_KEEPCNT failed");
    //     exit(EXIT_FAILURE);
    // }
}