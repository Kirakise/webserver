#include "Response.hpp"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unordered_map>
#include <cstdio>

int writeContent(std::string content, std::string path)
{
	std::ofstream	file;

	if (!isDirectory(path))
	{
		file.open(path.c_str());
		file << content;
		file.close();
		return (204);
	}
	else
	{
		file.open(path.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (file.is_open() == false)
			return (403);
		file << content;
		file.close();
		return (201);
	}
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
        code = 200;
        Content = readFile(pars.path);
        Content_type = getType(pars.path);
    }
}

void Response::DELETE()
{
    if (checkIfExists(pars.path))
    {
        std::remove(pars.path.c_str());
        if(!std::ifstream(pars.path))
            code = 200;
        else
            code = 403;
    }
    else
        code = 404;
}

void Response::POST()
{
    std::ofstream f;
    if (checkIfExists(pars.path))
    {
        f.open(pars.path, std::ofstream::app);
        if (f.good()) {
            code = 200;
            f << pars.body;
            f.close();
        }
        else
            code = 403;
    }
    else
    {
        f.open(pars.path);
        if (f.good()){
            code = 200;
            f << pars.body;
            f.close();
        } else
            code = 403;
    }

}


std::string getType(std::string path)
{
    std::string type;
	type = path.substr(path.rfind(".") + 1, path.size() - path.rfind("."));
	if (type == "html")
		return "text/html";
	else if (type == "css")
		return "text/css";
	else if (type == "js")
		return "text/javascript";
	else if (type == "jpeg" || type == "jpg")
		return "image/jpeg";
	else if (type == "png")
		return "image/png";
	else if (type == "bmp")
		return "image/bmp";
	else
		return "text/plain";
}

std::string Response::getCodeText(uint8_t code)
{
    std::string s;
    if (code == 200)
        return "OK";
    else if (code == 404){
        s = "res/404.html";
        Content_type = "text/html";
        Content = readFile(s);
        return "Not found";
    }
    else if (code == 403){
        s = "res/403.html";
        Content_type = "text/html";
        Content = readFile(s);
        return "Forbidden";
    }
    else if (code == 400){
        s = "res/400.html";
        Content_type = "text/html";
        Content = readFile(s);
        return "Bad request";
    }
}

std::string Response::getResponse(){
    std::string ret;

    ret == "HTTP/1.1 " + std::to_string(code) + getCodeText(code) + "\r\n";
    if (Content != "")
    {
        ret.append("Content-Type " + Content_type + "\r\n");
        ret.append("Content-Length " + std::to_string(Content.size()) + "\r\n\r\n");
        ret.append(Content);
    }
}