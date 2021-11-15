#pragma once
#include "ServerConf.hpp"


class ServerBlock{
    public:
    ServerBlock();
    ~ServerBlock();

    
     class ServerBadConfig : public std::exception{
        virtual const char* what() const throw(){
            return "bad config";
        }
    };
};