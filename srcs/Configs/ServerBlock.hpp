#ifndef SEVERBLOCK_HPP
# define SEVERBLOCK_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "ServerConf.hpp"
#include "Location.hpp"

class ServerConf;

class ServerBlock
{
    public:
    ServerBlock();
    ~ServerBlock();

    bool badConfig;
    size_t serverCount;
    std::string file;
    bool flag;
    
    class BadConfig : public std::exception{
        virtual const char* what() const throw(){
            return "bad config";
        }
    };

    void ServerCount() throw (BadConfig);
    void ParseAll(); //ex Fillthatshitpls
    void SplitServers() throw (BadConfig);
    
    void ParseTokens(std::string str, size_t n) throw (BadConfig);

    void listen(std::string str, size_t n) throw (BadConfig);
    void serverName(std::string str, size_t n) throw (BadConfig);
    void root(std::string str, size_t n) throw (BadConfig);
    void index(std::string str, size_t n) throw (BadConfig);
    void allow_methods(std::string str, size_t n) throw (BadConfig);
    void locations(std::string str, size_t n) throw (BadConfig);
    void closed_scope(std::string str, size_t n) throw (BadConfig);


    std::vector < ServerConf > servers;
    ServerConf *NewServer();

    static bool isempty(std::string str){
        size_t i = 0;
        while (i < str.length())
        {
            if (str[i] != ' ' && str[i] != '\t')
                return false;
            i++;
        }
        return true;
    }

    

};

#endif