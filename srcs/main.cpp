#include "Server.hpp"

int main(){
    Server *serv;
    try{
        std::cout << "228";
        serv = new Server(228);
    }
    catch ( std::exception &e)
    {
        std::cout << "229";
        serv = new Server(229);
    }
    
    std::cout << serv->port;
    
    try {
    serv->run();
    }
    catch ( std::exception &e )
    {
        std::cout << e.what();
    }
    return (0);
}