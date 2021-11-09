#include "../includes/Parser.hpp"
#include <string>


Parser::Parser(std::string request) : request(request){}

Parser::~Parser(){}

void Parser::splitRequest(){
    std::size_t found = this->request.rfind("\r\n\r\n");
    if (found != std::string::npos)
    {
        this->splited_req[1] = this->request.substr(found+4);
    }
        this->splited_req[0] = this->request.substr(0, found);
    std::cout << splited_req[0] << std::endl; // print debug
    std::cout << "1111" << std::endl;// print debug
    std::cout << splited_req[1] << std::endl;// print debug
}

void Parser::parseLines(){
    std::size_t found = this->splited_req[0].find("\r\n");
    std::size_t start = 0;
    std::string line;
    std::string start_line;
    start_line = this->splited_req[0];
    while (found != std::string::npos)
    {
        line = start_line.substr(start, found);
        std::cout << "|" << line << "|" << std::endl; //print debug
        this->lines.push_back(line);
        start_line = start_line.substr(found + 2);
        found = start_line.find("\r\n");
    }
}

void Parser::parse(){
    std::vector<std::string> lines;
    std::size_t found = this->lines[0].find(" ");
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
        lines.push_back(line);
        start_line = start_line.substr(found + 1);
        found = start_line.find(" ");
    }
    this->method = lines[0];
    this->path = lines[1];
    this->version = lines[2];
    std::cout << this->method << " *** " << this->path<< " *** " << this->version << " *** \n";
}



// int main ()
// {
//     Parser parser("GET gsdhg hgdhsg\r\naaaaaaa\r\nbbbb\r\n");
//     parser.splitRequest();
//     if (parser.splited_req[1] == "")
//         std::cout << "no content\n";
//     parser.parseLines();
//     parser.parse();
// }