#pragma once
#include "ServerConf.hpp"


class ServerBlock{
    public:
    ServerBlock();
    ~ServerBlock();
    ServerConf* servers;

    bool badConfig;
    size_t serverCount;
    std::string file;


    class BadConfig : public std::exception{
        virtual const char* what() const throw(){
            return "bad config";
        }
    };

    void ServerCount() throw (BadConfig);
    void MakeServerArray();
    void ParseServers() throw (BadConfig);
    void FillThatShitPls();
};