#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "ServerBlock.hpp"



class ServerConf{

    public:
    ServerConf();
    ~ServerConf();

    uint16_t _port;
    uint64_t _host; //?
    std::string _root;
    std::string* _serverName;
    bool _autoindex;
    std::vector<std::string> _indexes;
    std::string _cgi;
    std::vector<std::string> _cgiParams;
    ServerConf();
    void FillThatShitPls();
    uint16_t getPort() { return _port; }
    uint64_t getHost() { return _host; }

    

    ServerBlock* servers;

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
    void ParseServers();
};