#pragma once

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <list>

#include "../srcs/Configs/ServerBlock.hpp"


class Parser{

    public:

        Parser(std::string request, ServerBlock sv);
        ~Parser();

        // request fields:

        std::string                                        request;
        std::string                                        method;
        std::string                                        version;
        std::string                                        path;
        // std::vector<std::pair<std::string, std::list<std::string> > > headers;
        std::map<std::string, std::string>                 _headers;
        std::string                                        body;
        bool                                               bad_responce;
        

        std::string                                        host_name;
        uint16_t                                           port;
        ServerBlock                                        sv;

        // utils fields:

        std::vector<std::string>                lines;
        std::string                             splited_req;
        int                                     server_index;

        void parseStartLine();
        void parseHeaders();
        void parseLines();
        void splitRequest();
        void parseAll();
        void printVector( std::vector<std::pair<std::string, std::list<std::string> > >);
        std::string trimLine(std::string str, std::string totrim);
        std::list<std::string> splitLine(const char *str, const char * sep);
        void find_index();
        void find_host_port();
        std::string getMethod(){return method;};
        std::string getPath(){return path;};

};