#include "../includes/Parser.hpp"
#include <string>


Parser::Parser(std::string request) : request(request), bad_responce(false) {}

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
    std::string start_line;
    start_line = this->splited_req;
    while (found != std::string::npos)
    {
        line = start_line.substr(start, found);
        std::cout << "|" << line << "|" << std::endl; //print debug
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
        std::cout << "|" << line << "|" << std::endl; //print debug
        if (line != "")
            lines.push_back(line);
        start_line = start_line.substr(found + 1);
        found = start_line.find(" ");
    }
    this->method = lines[0];
    this->path = lines[1];
    this->version = lines[2];
    std::cout << this->method << " *** " << this->path<< " *** " << this->version << " *** \n";
    if (lines[0] == "" || lines[1] == "" || lines[2] == "")
        bad_responce = true;
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
    n = 1;
    while (n < lines.size()){
        
    }
}




int main ()
{
    Parser parser("GETgsdhg\r\n");
    parser.splitRequest();
    if (parser.body == "")
        std::cout << "no content\n";
    parser.parseLines();
    parser.parseStartLine();
    parser.parseHeaders();
}