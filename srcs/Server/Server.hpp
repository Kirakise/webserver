#pragma once
#include <unordered_map>
#include <string>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "Ultimate.hpp"

#define READ_SIZE 1024

class Server{
    public:
    Server(int port, std::string host);
    Server(ServerConf conf);
    ~Server();
    int setServer();
    int accept(void);
    int runServer(void);
    int recv(uint64_t socket);
    int send(uint64_t socket);
    void doRecv(uint64_t socket);
    void closeSocket(uint64_t socket);
    void closeServer(void);
    void getAllChunks(uint64_t socket);
    int64_t getFD(void);


    private:
    std::unordered_map<uint64_t, std::string> _requests;
    int64_t _fd;
    int _port;
    unsigned int _host;
    ServerConf _conf;
    struct sockaddr_in _addr;
};

