    #ifndef SERVERCONF_HPP
    #define SERVERCONF_HPP
    
    #include "ServerBlock.hpp"
    #include "Location.hpp"
    
    class ServerConf
    {
        public:


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
        // std::string serverText;
        std::vector <std::string> allowedMethods;
        bool ready;
        bool open_scope;
        std::vector < Location > locs;
        size_t loc_number;
    
    // uint16_t getPort() { return _port; }
    // uint64_t getHost() { return _host; }

      
    };

    #endif