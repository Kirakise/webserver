#include "Response.hpp"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unordered_map>
#include <cstdio>
#include "../Cgi/Cgi.hpp"

CodeToStr cts;

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

std::string getFilePathInLoc(std::string path, std::string locpath)
{
    int size = path.find(locpath) + locpath.size();
    return path.substr(size, path.size());
}

std::string isIndexed(std::string &path, ServerConf &conf, Location *l = 0)
{
    int loc = -1;
    bool find = false;
    if (l == 0) //Если мы не внутри location
        for (int i = 0; i < conf.locs.size(); i++)
        {
            if (conf.locs[i].locations[0][0] == '*' && path.find(conf.locs[i].locations[0].substr(conf.locs[i].locations[0].find("*")))
            && loc == -1)
                loc = i;
            if (path.find(conf.locs[i].locations[0]) != std::string::npos &&
            (loc == -1 || conf.locs[i].locations[0].size() > conf.locs[loc].locations[0].size()))
                loc = i;
        }
    else // Если мы смотрим другой локейшн
        for (int i = 0; i < l->locs.size(); i++)
        {
            if (l->locs[i].locations[0][0] == '*' && path.find(l->locs[i].locations[0].substr(l->locs[i].locations[0].find("*")))
            && loc == -1)
                loc = i;
            if (path.find(l->locs[i].locations[0]) != std::string::npos &&
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
            {
                path = conf._root + path;
                goto end;
            }
            else
                for (int i = 0; i < conf._indexes.size(); i++)
                    if (conf._indexes[i] == path || "/" + conf._indexes[i] == path)
                    {
                        path = conf._root + path;
                        goto end;
                    }
            path = "";
        }
        else {
            if (l->autoindex) {
                path = l->_root + getFilePathInLoc(path, l->locations[0]);
                goto end;
            }
            else if (l->locations[0][0] == '*'){
                path = l->_root.substr(0, l->_root.find("*")) + getFilePathInLoc(path, l->locations[0].substr(0, l->locations[0].find("*")));
                goto end;
            }
            else
                for (int i = 0; i < l->indexes.size(); i++)
                    if (l->indexes[i] == getFilePathInLoc(path, l->locations[0]) || 
                        "/" + l->indexes[i] == getFilePathInLoc(path, l->locations[0]))
                        {
                        path =  l->_root + getFilePathInLoc(path, l->locations[0]);
                        goto end;
                        }
            path = "";
        }
    }
    end:
    if (l != 0){
        conf.allowedMethods = l->allowedMethods;
        conf._cgi = l->_cgi_pass;
        conf.clientBodySize = l->clientBodyBufferSize;
        conf.redir = l->redir;
    }
    if (conf.redir.size())
        return ("123");
    else
        return path;
}

void Response::Execute()
{
    if (pars.bad_responce) { code = 400; return ; }
    if (pars.method == "GET")
        GET();
    else if (pars.method == "POST" || pars.method == "PUT")
        POST();
    else if (pars.method == "DELETE")
        DELETE();
    else { code = 501; return;}
}

bool isAllowed(const std::string &method, std::vector <std::string> &methods)
{
    for (int i = 0; i < methods.size(); i++)
        if (method == methods[i])
            return true;
    return false;
}

void Response::GET()
{
    if ((pars.path = isIndexed(pars.path, Conf)) == "") { code = 403; return ; }
    if (Conf.redir != "") { code = 307; return ; }
    if (!isAllowed("GET", Conf.allowedMethods)) { code = 405; return ;}
    if (!checkIfExists(pars.path)) { code = 404; return ; }
    if (Conf.cgi_pass.size() != 0)
    {
        Cgi c(*this);
        c.startCgi();
        Content = c.body;
        Content_type = getType(pars.path);
        code = 200;
        return ;
    }
    else
    {
        if (isDirectory(pars.path)){
            code = 200;
            Content_type = "text/html";
            Content = getListingResponse(pars.path);
        }
        else{
            Content = readFile(pars.path);
            code = 200;
            Content_type = getType(pars.path);
        }
    }
}

void Response::DELETE()
{
    if ((pars.path = isIndexed(pars.path, Conf)) == "") { code = 403; return ; }
    if (Conf.redir != "") { code = 307; return ; }
    if (!isAllowed("DELELTE", Conf.allowedMethods)) { code = 405; return ;}
    if (checkIfExists(pars.path))
    {
        std::remove(pars.path.c_str());
        if(!std::ifstream(pars.path))
            code = 204;
        else
            code = 403;
    }
    else
        code = 404;
}

void Response::POST()
{
    std::ofstream f;
    isIndexed(pars.path, Conf);
    if (isDirectory(pars.path) && 
    pars._headers["Content-Disposition"].find("filename") != std::string::npos )
    {
        if (pars.path[pars.path.size() - 1] != '/')
            pars.path.append("/");
        pars.path += pars._headers["Content-Disposition"].substr(pars._headers["Content-Disposition"].find("filename=") + 10);
        pars.path.erase(pars.path.size() - 1);
    }
    if (Conf.redir != "") { code = 307; return ; }
    if (!isAllowed("POST", Conf.allowedMethods)) { code = 405; return ;}
    //if ((pars.path = isIndexed(pars.path, Conf)) == "") { code = 403; return ; }
    if (Conf.clientBodySize != -1 && Conf.clientBodySize < pars.body.size()) { code = 413; return;}
    if (Conf.cgi_pass.size() != 0)
    {
        Cgi c(*this);
        c.startCgi();
        Content = c.body;
        Content_type = getType(pars.path);
        code = 200;
        return ;
    }
    if (checkIfExists(pars.path))
    {
        f.open(pars.path, std::ofstream::app);
        if (f.good()) {
            code = 204;
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
            code = 204;
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
    if (code == 200)
        return "OK";
    else if (code == 307)
    {
        headers.push_back("Location: " + Conf.redir);
        return cts.error_messages[code];
    }
    else {
        if (!checkIfExists(Conf.error_page[code])) {
            Conf.error_page[code] = "res/" + std::to_string(code) + ".html"; 
        }
        Content_type = getType(Conf.error_page[code]);
        Content = readFile(Conf.error_page[code]);
        return cts.error_messages[code];
    }
}

std::string Response::getResponse(){
    std::string ret;

    ret = "HTTP/1.1 " + std::to_string(code) + " " + getCodeText(code) + "\r\n";
    for (int i = 0; i < headers.size(); i++)
        ret.append(headers[i] + "\r\n");
    if (Content != "" || pars.method == "GET")
    {
        ret.append("Content-Type: " + Content_type + "\r\n");
        ret.append("Content-Length: " + std::to_string(Content.size()) + "\r\n\r\n");
        ret.append(Content);
    }
    else 
        ret.append("\r\n");
    return ret;
}