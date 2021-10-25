#include "Server.hpp"

int main(){
    Server serv(228);

    try {
    serv.run();
    }
    catch ( std::exception &e )
    {
        std::cout << e.what();
    }
    return (0);
}