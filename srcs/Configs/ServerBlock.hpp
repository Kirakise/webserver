#ifndef SEVERBLOCK_HPP
# define SEVERBLOCK_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#include "ServerConf.hpp"
#include "Location.hpp"

# define LISTEN 0
# define NAME 1
# define ROOT 2
# define INDEX 3
# define METHODS 4
# define LOCATION 5
# define ENDSCOPE 6
# define AUTOINDEX 7
# define REDIR 8
# define CGI 9
# define DEFAULT 10

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

    void listen(std::string str, size_t n) throw (BadConfig); //0
    void serverName(std::string str, size_t n) throw (BadConfig); //1
    void root(std::string str, size_t n) throw (BadConfig); //2
    void index(std::string str, size_t n) throw (BadConfig); //3
    void allow_methods(std::string str, size_t n) throw (BadConfig); //4
    void locations(std::string str, size_t n) throw (BadConfig); 
    void closed_scope(std::string str, size_t n) throw (BadConfig);
    void parse_autoindex(std::string str, size_t n) throw (BadConfig);
    void parse_error_page(std::string str, size_t n) throw (BadConfig);
    void parse_redirect(std::string str, size_t n) throw (BadConfig);
    void parse_cgi(std::string str, size_t n) throw (BadConfig);
    void parse_default_page(std::string str, size_t n) throw (BadConfig);
    bool checkExists(std::string path);


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