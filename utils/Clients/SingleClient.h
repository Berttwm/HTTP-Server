#ifndef SingleClient_H
#define SingleClient_H

#include "../Servers/HTTP_Tools/HTTP_Request.h"
#include "Client.h"

class SingleClient :  public Client { 
private:
    /* Fixed HTTP Client Config */
    int Server_Port = 12346; // change accordingly
    int Client_Port = 15000;
public:
    SingleClient();
    void launch();
};

#endif // SingleClient_H