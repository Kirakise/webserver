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

std::string getType(std::string path);