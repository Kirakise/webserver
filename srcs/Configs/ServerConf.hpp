    #ifndef SERVERCONF_HPP
    #define SERVERCONF_HPP
    
    #include "ServerBlock.hpp"
    #include "Location.hpp"
    #include <unordered_map>
    
    class ServerConf
    {
        public:


        ServerConf() : _port(80), host("127.0.0.1"), _autoindex(false), clientBodySize(-1), loc_number(0){
            size_t i = 0;
            while (i < 8){
                this->type_index[i] = false;
                i++;
            }
            error_page[400] = "res/400.html";
            error_page[403] = "res/403.html";
            error_page[404] = "res/404.html";
            error_page[405] = "res/405.html";
            error_page[413] = "res/413.html";
            error_page[500] = "res/500.html";
            error_page[501] = "res/501.html";
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
        long long clientBodySize;
        std::vector < Location > locs;
        size_t loc_number;
        std::unordered_map <int, std::string> error_page;
    
        uint16_t getPort() { return _port; }
        std::string getHost() { return host; }

        bool type_index[8];
    };

    #endif