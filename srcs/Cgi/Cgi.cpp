#include "Cgi.hpp"
#include <string>

Cgi::Cgi(Response res) : res(res), body(res.body), conf(res.Conf), req(res.pars), headers(res.pars._headers){}
Cgi::~Cgi(){}


void Cgi::make_env(){
    env["CONTENT_LENGTH"] = std::to_string(body.length()); //c++ 11, replace?
    env["SERVER_SOFTWARE"] = "Webserv/1.0";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_PORT"] = std::to_string(conf.getPort());
    env["REQUEST_METHOD"] = req.getMethod();
    env["PATH_INFO"] = req.getPath();
    env["PATH_TRANSLATED"] = req.getPath();
    env["SCRIPT_NAME"] = conf.cgi_pass;
    // env["QUERY_STRING"] = ; //???????????????
    env["REMOTE_ADDR"] = conf.host;
    env["REMOTE_USER"] = headers["Authorization"];
    env["REMOTE_IDENT"] = headers["Authorization"];
    env["AUTH_TYPE"] = headers["Authorization"];
    env["SERVER_NAME"] = headers["Hostname"];
    env["CONTENT_TYPE"] = headers["Content-Type"];
}

void Cgi::env_to_array()
{
    char * env_arr[env.size()];
    size_t j = 0;
    std::string s;
    for (std::map<std::string, std::string>::iterator i = env.begin(); i != env.end(); i++){
        s = i->first + "=" + i->second;
        env_arr[j] = strdup(s.c_str());
        j++;
    }
    env_ready = env_arr;
}