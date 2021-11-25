#pragma once
#include "Server.hpp"
#include "Ultimate.hpp"

struct Server;

class Cluster{
    public:
    ~Cluster();
    int setup();
    void run();
    void clear();
    Cluster(std::vector <ServerConf> &);
    private:
    std::vector <Server> _servers;
    std::unordered_map <uint64_t, Server *> _serverMap;
    std::vector <uint64_t> _answer;
    fd_set  _fd_set;
    uint64_t _fd_max;
};