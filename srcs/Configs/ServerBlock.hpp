#pragma once
#include "ServerConf.hpp"


class ServerBlock{
    public:
    ServerBlock();
    ~ServerBlock();

    bool badConfig;
    size_t serverCount;
    std::string file;
    std::vector < ServerConf > servers;


    class BadConfig : public std::exception{
        virtual const char* what() const throw(){
            return "bad config";
        }
    };

    void ServerCount() throw (BadConfig);
    void ParseServers() throw (BadConfig);
    void ParseAll(); //ex Fillthatshitpls
    void SplitServers() throw (BadConfig);
    ServerConf *NewServer();
    void ParseTokens(std::string str, size_t n) throw (BadConfig);

    void listen(std::string str, size_t n) throw (BadConfig);
    void serverName(std::string str, size_t n) throw (BadConfig);
    void root(std::string str, size_t n) throw (BadConfig);
    void index(std::string str, size_t n) throw (BadConfig);
    void allow_methods(std::string str, size_t n) throw (BadConfig);
    void locations(std::string str, size_t n) throw (BadConfig);
    void open_scope(std::string str, size_t n) throw (BadConfig);
    void closed_scope(std::string str, size_t n) throw (BadConfig);
};