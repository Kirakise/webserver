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

std::string getPath(std::string path)
{
    int i = path.size() - 1;

    while (path[i] != '/')
        i--;
    return path.substr(0, i);
}

std::string getFile(std::string path)
{
    int i = path.size() - 1;
    while (path[i] != '/');
}

std::string getFilePathInLoc(std::string path, std::string locpath)
{
    int size = path.find(locpath) + locpath.size();
    return path.substr(size, path.size());
}

std::string isIndexed(std::string &path, ServerConf conf, Location *l = 0)
{
    int loc = -1;
    bool find = false;
    if (l == 0) //Если мы не внутри location
        for (int i = 0; i < conf.locs.size(); i++)
        {
            if (getPath(path).find(conf.locs[i].locations[0]) != std::string::npos &&
            (loc == -1 || conf.locs[i].locations[0].size() > conf.locs[loc].locations[0].size()))
                loc = i;
        }
    else // Если мы смотрим другой локейшн
        for (int i = 0; i < l->locs.size(); i++)
        {
            if (getPath(path).find(l->locs[i].locations[0]) != std::string::npos &&
            (loc == -1 || l->locs[i].locations[0].size() > l->locs[loc].locations[0].size()))
                loc = i;
        }
    
    if (loc != -1) { //Если нашлась лока
        if (l == 0)
            return isIndexed(path, conf, &conf.locs[loc]);
        else
            return isIndexed(path, conf, &l->locs[loc]);
    }
    else{
        if (l == 0) {
            if (conf._autoindex)
                return conf._root + path;
            else
                for (int i = 0; i < conf._indexes.size(); i++)
                    if (conf._indexes[i] == path || "/" + conf._indexes[i] == path)
                        return conf._root + path;
            return "";
        }
        else {
            if (false) {}
            else
                for (int i = 0; i < l->indexes.size(); i++)
                    if (l->indexes[i] == getFilePathInLoc(path, l->locations[0]) || 
                        "/" + l->indexes[i] == getFilePathInLoc(path, l->locations[0]))
                        return l->_root + getFilePathInLoc(path, l->locations[0]);
            return "";
        }
    }
}

void Response::Execute()
{
    if (pars.bad_responce) { code = 400; return ; }
    if (pars.method == "GET")
        GET();
    else if (pars.method == "POST")
        POST();
    else if (pars.method == "DELETE")
        DELETE();
}

void Response::GET()
{
    if ((pars.path = isIndexed(pars.path, Conf)) == "") { code = 403; return ; }
    if (!checkIfExists(pars.path)) { code = 404; return ; }
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
    if ((pars.path = isIndexed(pars.path, Conf)) == "") { code = 403; return ; }
    if (!checkIfExists(pars.path))
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
    if ((pars.path = isIndexed(pars.path, Conf)) == "") { code = 403; return ; }
    if (!checkIfExists(pars.path))
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

std::string Response::getCodeText(uint16_t code)
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
    else return "Internal error";
}

std::string Response::getResponse(){
    std::string ret;

    ret = "HTTP/1.1 " + std::to_string(code) + " " + getCodeText(code) + "\r\n";
    if (Content != "")
    {
        ret.append("Content-Type: " + Content_type + "\r\n");
        ret.append("Content-Length: " + std::to_string(Content.size()) + "\r\n\r\n");
        ret.append(Content);
    }
    else 
        ret.append("\r\n");
    std::cout << ret;
    return ret;
}