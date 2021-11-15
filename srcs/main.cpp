#include "Ultimate.hpp"
#include <iostream>
int main(){
    Cluster clust;
    try{
        std::cout << "228";
        serv = new Server(228, AF_INET);
    }
    catch ( std::exception &e)
    {
        std::cout << "229";
        serv = new Server(229, AF_INET);
    }
    
    std::cout << serv->port;
    
    serv
    return (0);
}