#ifndef ListeningSocket_H
#define ListeningSocket_H

#include "BindingSocket.h"

class ListeningSocket : public BindingSocket
{
private:
    int backlog;
    int listening;
public:
    ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bklog);
    void start_listening();
};

#endif