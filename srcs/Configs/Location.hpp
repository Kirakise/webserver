#pragma once

#include "ServerBlock.hpp"
#include "ServerConf.hpp"

class Location{

    public:

    Location();
    ~Location();

    Location *locations;
    std::vector < std::string> locTokens;
    std::string alias;
    bool alias;
    std::string root;
    bool root;
    std::vector < std::string > allowedMethods;
    std::string index;
    std::string cgi_pass;
    size_t clientBodyBufferSize;

    void ParseLocTokens();
    void ParseAlias();
    void ParseRoot();
    void ParseLocation();
    void ParseAllowedMethods();
    void ParseCgiPass();

};