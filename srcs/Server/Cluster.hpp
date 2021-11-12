#pragma once
#include "Server.hpp"
#include "ServerConf.hpp"

struct t_conf;

class Cluster{
    public:
    Cluster(t_conf f);
    ~Cluster();
    int setup();
    void run();
    void clear();
    private:
    Cluster(std::vector <ServerConf> &);
    std::vector <Server> _servers;
    std::unordered_map <uint64_t, Server *> _serverMap;
    std::vector <uint64_t> _answer;
    fd_set  _fd_set;
    uint64_t _fd_max;
};