#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "ServerBlock.hpp"
#include "ServerConf.hpp"

class Location{

    public:

    Location();
    ~Location();

    std::vector < std::string> locTokens;
    std::string alias;
    bool _alias;
    std::string root;
    bool _root;
    std::vector < std::string > allowedMethods;
    std::string index;
    std::string cgi_pass;
    size_t clientBodyBufferSize;
};

#endif