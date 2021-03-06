#include "../../includes/Parser.hpp"
#include <string>
#include <iostream>

Parser::Parser(std::string request, ServerBlock sv) : request(request), bad_responce(false), sv(sv) {
    this->splitRequest();
    // if (this->body == "")
    //     std::cout << "no content\n";
    this->parseLines();
    this->parseStartLine();
    this->parseHeaders();
    this->find_index();
    std::cout << request << std::endl;
    for (std::map<std::string, std::string>::iterator i = this->_headers.begin(); i != this->_headers.end(); i++)
    {
        std::cout << "---" << i->first << "---" << i->second << std::endl;
    }
}

Parser::~Parser(){}

void Parser::splitRequest(){
    std::size_t found = this->request.rfind("\r\n\r\n");
    if (found != std::string::npos)
    {
        this->body = this->request.substr(found+4);
    }
    else
    {
        this->bad_responce = true;
    }
    this->splited_req = this->request.substr(0, found);
}

void Parser::parseLines(){
    std::size_t found = this->splited_req.find("\r\n");
    std::size_t start = 0;
    std::string line;
    std::cout << request << std::endl;
    std::string start_line;
    start_line = this->splited_req;
    while (found != std::string::npos)
    {
        line = start_line.substr(start, found);
        // std::cout << "|" << line << "|" << std::endl; //print debug
        this->lines.push_back(line);
        start_line = start_line.substr(found + 2);
        found = start_line.find("\r\n");
    }
}

void Parser::parseStartLine(){
    std::vector<std::string> lines;
    std::size_t found = this->lines[0].find(" ");
    if (found == std::string::npos)
    {
        this->bad_responce = true;
        return ;
    }
    std::size_t start = 0;
    std::string line;
    std::string start_line;
    start_line = this->lines[0];
    bool flag = true;
    while (found != std::string::npos || flag)
    {
        if (found == std::string::npos)
            flag = false;
        line = start_line.substr(start, found);
        // std::cout << "|" << line << "|" << std::endl; //print debug
        if (line != "")
            lines.push_back(line);
        start_line = start_line.substr(found + 1);
        found = start_line.find(" ");
    }
    this->method = lines[0];
    this->path = lines[1];
    this->version = lines[2];
    // std::cout << this->method << " *** " << this->path<< " *** " << this->version << " *** \n";
    if (lines[0] == "" || lines[1] == "" || lines[2] == "")
        bad_responce = true;
}

std::list<std::string> Parser::splitLine(const char *str, const char * sep){
    std::list<std::string> list;
    char * token = std::strtok((char *)str, sep);
    std::string string;
    while (token){
        string = token;
        // std::cout << "-" << string << std::endl;
        string = trimLine(string, ";q=");
        list.push_back(string);
        token = std::strtok(NULL, sep);
    }
    return list;
}

void Parser::parseHeaders(){
    if (this->bad_responce)
        return;
    if (this->lines.size() == 1)
    {
        std::cout << "end\n";
        return;
    }
    size_t n;
    size_t found;
    n = 1;
    std::string key;
    std::string afterkey;
    std::list <std::string> value;
    std::pair<std::string, std::string> p1;
    while (n < this->lines.size()){ 
        found = this->lines[n].find(":");
        if (found == std::string::npos)
        {
            this->bad_responce = true;
            return;
        }
        key = this->lines[n].substr(0, found);
        afterkey = this->lines[n].substr(found+1);
        p1 = std::make_pair(key, afterkey);
        this->_headers.insert(p1);
        n++;
    }
    // printVector(this->headers);
}

// void Parser::printVector( std::vector<std::pair<std::string, std::list<std::string> > > vec){
//     for(size_t i = 0;  i < vec.size(); i++)
//     {
//         std::cout << i << ": " << vec[i].first << "| ";
//         for (std::string s : vec[i].second)
//             std::cout << s << "||";
//         std::cout << std::endl;
//     }
// }

std::string Parser::trimLine(std::string str, std::string totrim)
{
    size_t found;
    found = str.find(totrim);
    if (found == std::string::npos)
        return str;
    else
       str = str.substr(0, found);
    std::cout << "(" << str << ")" << std::endl;
    return (str);
}


// int main ()
// {
//     Parser parser("GET / HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\nAccept-Language: en-GB,en;q=0.5\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\n\r\n eghgfhgfheghegferhfegrhfgerhegfheg");
//     // parser.splitRequest();
//     // if (parser.body == "")
//     //     std::cout << "no content\n";
//     // parser.parseLines();
//     // parser.parseStartLine();
//     // parser.parseHeaders();
// }

void Parser::find_host_port(){
    size_t f;
    f = this->request.find("Host: ");
    if (f == std::string::npos){
        bad_responce = true;
        return;
    }
    std::string word;
    f+=6;
    while (this->request[f] == ' ')
        f++;
    while (this->request[f] != ':' && f < this->request.length())
    {
        word+=this->request[f];
        f++;
    }
    if (this->request[f] != ':' && f == this->request.length())
    {
        this->host_name = word;
        this->port = 80;
        return;
    }
    f++;
    this->host_name = word;
    word = "";
    while (f < this->request.length())
    {
        word += this->request[f];
        f++;
    }
    this->port = atoi(word.c_str());
}

void Parser::find_index(){
    find_host_port();
    size_t i = 0;
    std::vector<int> indexes;
    while (i < sv.servers.size())
    {
        if (sv.servers[i]._port == this->port)
            indexes.push_back(i);
        i++;
    }
    if (indexes.size() == 0)
        this->server_index = 0;
    if (indexes.size() == 1)
        this->server_index = indexes[0];
    i = 0;
    while (i < indexes.size())
    {
        if (this->host_name == sv.servers[indexes[i]].host)
        {
            this->server_index = indexes[i];
            return ;
        }
        i++;
    }
    this->server_index = indexes[0];
}