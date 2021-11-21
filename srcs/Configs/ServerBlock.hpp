#ifndef SEVERBLOCK_HPP
# define SEVERBLOCK_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>


class ServerBlock
{
    public:
    ServerBlock();
    ~ServerBlock();

    bool badConfig;
    size_t serverCount;
    std::string file;
    
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

    class ServerConf
    {
        public:

              class Location{

            public:

            Location() : _alias(false), _root(false), ready(false){} ;
            ~Location(){};

            std::vector < std::string> locations;
            std::string alias;
            bool _alias;
            std::string root;
            bool _root;
            std::vector < std::string > allowedMethods;
            std::string index;
            std::string cgi_pass;
            size_t clientBodyBufferSize;
            bool ready;
            std::vector < Location > locs;
        
            void ParseLocation();
            static Location* newLocation() {Location *newloc = new Location; return newloc;};
        };

        ServerConf() : _port(80), host("default"), _autoindex(false), loc_number(0){};
        ~ServerConf(){};

        uint16_t _port;
        uint64_t _host; //?
        std::string host; //to set default or parse address
        std::string _root;
        std::vector < std::string >  _serverName;
        bool _autoindex;
        std::vector<std::string> _indexes;
        std::string _cgi;
        std::vector<std::string> _cgiParams;
        std::string serverText;
        std::vector <std::string> allowedMethods;
        bool ready;
        bool open_scope;
        std::vector < Location > locs;
        size_t loc_number;
    
    // uint16_t getPort() { return _port; }
    // uint64_t getHost() { return _host; }

      
    };

    std::vector < ServerConf > servers;
    ServerConf *NewServer();

    bool isempty(std::string str){
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