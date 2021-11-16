#include "ServerConf.hpp"

ServerConf::ServerConf() : _port(80), host("default"), _autoindex(false){}

void ServerConf::ParseAll(){
    try {

    }
    catch(std::exception const & ex){
        this->badConfig = true;
    }
}

void ServerConf::ParseListen() throw (BadConfig){
    
}
