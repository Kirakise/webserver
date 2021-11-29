#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "Ultimate.hpp"
std::string readFile(std::string& filename);

std::string getListingResponse(std::string path);

std::string getMimeType(std::string path);

int isDirectory(std::string path);

class Response{
    public:
    uint16_t code;
    uint64_t Content_length;
    std::string Content_type;
    std::string Content;
    std::vector <std::string> headers;
    Parser pars;
    ServerConf Conf;

    Response(Parser p, ServerConf Conf) : pars(p), Conf(Conf) {}
    void Execute();
    void GET();
    void POST();
    void DELETE();
    std::string getCodeText(uint16_t code);
    std::string getResponse();
};

class CodeToStr{
    public:
    CodeToStr() {
        error_messages[301] = "Moved Permanently";
        error_messages[400] = "Bad Request";
        error_messages[403] = "Forbidden";
        error_messages[404] = "Not Found";
        error_messages[405] = "Method Not Allowed";
        error_messages[413] = "Payload Too Large";
        error_messages[500] = "Internal Error";
        error_messages[501] = "Not Implemented";
    }
    ~CodeToStr(){}
    std::unordered_map <int, std::string> error_messages;

};

std::string getType(std::string path);