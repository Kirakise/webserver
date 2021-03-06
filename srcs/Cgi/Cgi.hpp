#ifndef CGI_HPP
# define CGI_HPP

#include "Ultimate.hpp"



class Cgi {
    public :
    Cgi(Response res);
    ~Cgi();

    std::map<std::string, std::string> env;
    char ** env_ready;
    Response res;
    std::string body;
    ServerConf conf;
    Parser req;
    std::map<std::string, std::string> headers;

    void make_env();
    void env_to_array();
    void startCgi();
};

#endif