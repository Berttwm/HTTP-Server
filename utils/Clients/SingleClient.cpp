#include "SingleClient.h"

SingleClient::SingleClient()
: Client(AF_INET, SOCK_STREAM, 0, 12346, INADDR_ANY) // port 80 for HTTP
{
    launch();
}

void SingleClient::launch() {
    // generate response string
    std::string client_request_str = HTTP_Request::generate_HTTP_Request();
    std::cout << client_request_str << std::endl;
    char buffer[30000] = {0};

    if(write(get_socket()->get_sock(), client_request_str.c_str(), client_request_str.length()) < 0){
        perror("Cient write failure");
        exit(EXIT_FAILURE);
    }
    if(read(get_socket()->get_sock(), buffer, 30000) < 0)
    {
        perror("Cient read failure");
        exit(EXIT_FAILURE);
    }

    std::cout << "Client has sent standard HTTP message" << std::endl;


}