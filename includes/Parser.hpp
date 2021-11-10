#pragma once

#include <map>
#include <string>
#include <vector>


class Parser{

    public:

        Parser(std::string request);
        ~Parser();

        std::string                         request;

        std::string                         method;
        std::string                          version;
   
        std::map <std::string, std::string> headers;
        std::string                         path;


        std::vector<std::string>            lines;
        std::string                         splited_req[2];
    //  std::string                         content; // nahui
        void parse();
        // void parsePath(std::string request);
        void parseHeaders(std::string *split_req);
        void parseLines();
        void splitRequest();
};