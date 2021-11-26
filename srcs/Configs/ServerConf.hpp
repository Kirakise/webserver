    #ifndef SERVERCONF_HPP
    #define SERVERCONF_HPP
    
    #include "ServerBlock.hpp"
    #include "Location.hpp"
    
    class ServerConf
    {
        public:


        ServerConf() : _port(80), host("127.0.0.1"), _autoindex(false), loc_number(0){
            size_t i = 0;
            while (i < 8){
                this->type_index[i] = false;
                i++;
            }
        };
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
    
        uint16_t getPort() { return _port; }
        std::string getHost() { return host; }

        bool type_index[8];
    };

    #endif