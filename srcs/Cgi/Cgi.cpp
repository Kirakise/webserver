#include "Cgi.hpp"
#include <string>
#include <stdio.h>
#include <unistd.h>


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
    make_env();
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

void Cgi::startCgi(){
    int         save0;
    int         save1;
    std::string res;
    pid_t       pid;
    std::string cgi = this->conf.cgi_pass;
    int ret = 1;

    env_to_array();

    FILE *fin = tmpfile();
    FILE *fout = tmpfile();
    long fdin = fileno(fin);
    long fdout = fileno(fout);
    save0 = dup(0);
    save1 = dup(1);

    pid = fork();

    if (pid == -1){
        body = "Status: 500\r\n\r\n";
        return;
    }
    else if (pid == 0){
        dup2(fdin, 0);
        dup2(fdout, 1);
        execve(cgi.c_str(), NULL, env_ready);
        std::cerr << "Execve error" << std::endl;
        write(1, "Status: 500\r\n\r\n", 16);
    }
    else{
        char buf[10000] = {0};
        waitpid(-1, NULL, 0);
        lseek(fdout, 0, SEEK_SET);

        ret = 1;
        while (ret > 0){
            memset(buf, 0, 10000);
            ret = read(fdout, buf, 10000);
            res += buf;
        }
    }

    dup2(save0, 0);
    dup2(save1, 1);
    fclose(fin);
    fclose(fout);
    close(fdin);
    close(fdout);
    close(save0);
    close(save1);

    for(size_t i = 0; env_ready[i]; i++)
        delete [] env_ready[i];
    delete[] env_ready;

    if (!pid)
        exit(0);
    body = res;
}