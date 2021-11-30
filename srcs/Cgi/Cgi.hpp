#include "../Response/Response.hpp"


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
};