#pragma once

#include <map>
#include <string>
#include <vector>
#include <utility>


class Parser{

    public:

        Parser(std::string request);
        ~Parser();

        // request fields:

        std::string                                        request;
        std::string                                        method;
        std::string                                        version;
        std::string                                        path;
        std::vector<std::pair<std::string, std::string> >  headers;
        std::string                                        body;
        bool                                               bad_responce;

        // utils fields:

        std::vector<std::string>            lines;
        std::string                         splited_req;

        void parseStartLine();
        void parseHeaders();
        void parseLines();
        void splitRequest();
        void parseAll();

};