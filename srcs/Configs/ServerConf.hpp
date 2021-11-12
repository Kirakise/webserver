#pragma once
#include <string>
#include <vector>

class ServerConf{
    public:
    uint16_t _port;
    uint64_t _host;
    std::string _root;
    std::string _serverName;
    bool _autoindex;
    std::vector<std::string> _indexes;
    std::string _cgi;
    std::vector<std::string> _cgiParams;
    ServerConf();
    void FillThatShitPls();
    uint16_t getPort() { return _port; }
    uint64_t getHost() { return _host; }
};