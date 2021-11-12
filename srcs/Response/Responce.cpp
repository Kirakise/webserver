#include "Response.hpp"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unordered_map>

static std::unordered_map<std::string, std::string> MIMES;

std::string getMimeType(std::string path)
{
    std::string::iterator it = path.end() - 1;
    while (*it != '.' && it != path.begin())
        it--;
    std::string str = path.substr(path.begin() - it, path.size());
    
}


bool checkIfExists(std::string path)
{
    struct stat buffer;   
    return (stat (path.c_str(), &buffer) == 0);
}

bool isIndexed(std::string &path, std::vector <std::string> &index)
{
    for (int i = 0; i < index.size(); i++)
    {
        if (path == index[i])
            return true;
    }
    return false;
}

void Response::Execute()
{
    if (pars.bad_responce) { code = 400; return ; }
    if (!checkIfExists(pars.path)) { code = 404; return ; }
    if (pars.method == "GET")
        GET();
    else if (pars.method == "POST")
        POST();
    else if (pars.method == "DELETE")
        DELETE();
}

void Response::GET()
{
    if (!Conf._autoindex && !isIndexed(pars.path, Conf._indexes)) { code = 403; return ; }
    if (isDirectory(pars.path)){
        code = 200;
        Content_type = "text/html";
        Content = getListingResponse(pars.path);
    }
    else{
        
    }
}
