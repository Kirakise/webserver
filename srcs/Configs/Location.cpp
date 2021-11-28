#include "Location.hpp"
#include "ServerBlock.hpp"
#include <fstream>

extern size_t line_number;

Location::Location() :  autoindex(false), clientBodyBufferSize(-1) {};

Location::Location(bool autoindex) : autoindex(autoindex) {};
    
Location::~Location(){
    // this->allowedMethods.clear();
    // this->indexes.clear();
    // this->locations.clear();
    // this->locs.clear();
    // for (size_t i = 0; i < locs.size(); i++){
    //     delete locs[i];
    // }
}

void Location::ParseDirects(std::string buf){
    size_t i = 0;
    while (buf[i] == ' ' || buf[i] == '\t')
        i++;
    std::string word;
    word = "location";
    i += word.length() + 1;
    while ((buf[i] == ' ' || buf[i] == '\t') && i < buf.length())
        i++;
    while (i < buf.length() && buf[i] != '{'){
        word = "";
         while (buf[i] != ' ' && i < buf.length()){
            word += buf[i];
            i++;
        }
        this->locations.push_back(word);
        while (buf[i] == ' ' || buf[i] == '\t')
            i++;
    }
    if (buf[i] == '{')
        this->open_scope = true;
    else
        throw ServerBlock::BadConfig();
    int u = 0;
    while (u < this->locations.size()){
        std::cout << this->locations[u] << std::endl;
        u++;
    }
}

void Location::allow_methods(std::string str){
    
    size_t i = 0;
    while ((str[i] == ' ' || str[i] == '\t') && i < str.length())
        i++;
    std::string word;
    word = "allow_methods";
    i += word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    while (i < str.length()){
        word = "";
         while (str[i] != ' ' && i < str.length()){
            word += str[i];
            i++;
        }
        this->allowedMethods.push_back(word);
        while (str[i] == ' ' || str[i] == '\t')
        i++;
    }
    int u = 0;
    while (u < this->allowedMethods.size()){
        std::cout << this->allowedMethods[u] << std::endl;
        u++;
    }
}

void Location::parse_autoindex(std::string str){
    size_t i = 0;
    while ((str[i] == ' ' || str[i] == '\t') && i < str.length())
        i++;
     std::string word;
    word = "autoindex";
    i += word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    word = "";
    while (i < str.length() && i != ' ' && i != '\t'){
        word += str[i];
        i++;
    }
    if (word == "on")
        this->autoindex = true;
    else if (word == "off")
        this->autoindex = false;
    else
        throw ServerBlock::BadConfig();
}

void Location::parse_index(std::string str){
    size_t i = 0;
    while ((str[i] == ' ' || str[i] == '\t') && i < str.length())
        i++;
    std::string word;
    word = "index";
    i = word.length() + i;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    while (i < str.length()){
        word = "";
         while (str[i] != ' ' && i < str.length()){
            word += str[i];
            i++;
        }
        this->indexes.push_back(word);
        std::cout << word << "asdasdasdasd" << std::endl;
        while (str[i] == ' ' || str[i] == '\t')
        i++;
    }
}

void Location::ParseTokens(std::string str){
    std::string types[8] = {
                        "root",
                        // "index",
                        "allow_methods",
                        "cgi_pass",
                        "}",
                        "location",
                        "client_max_body_size",
                        "index",
                        "autoindex"};

    size_t i = 0;
    size_t j = 0;
    std::string word;
    if (str == "" || ServerBlock::isempty(str))
        return ;
    while (str[i] == '\t' || str[i] == ' ')
        i++;
    while (str[i] != ' ' && i < str.length())
    {
        word+=str[i];
        i++;
    }
    while (word != types[j] && j < 8)
        j++;
    if (j == 8)
        throw ServerBlock::BadConfig();
    typedef void(Location::*Parse)(std::string str);
    Parse word_parse[8] = {&Location::root, 
                    //  &Location::index, 
                     &Location::allow_methods,
                     &Location::cgi_pass,
                     &Location::closed_scope,
                     &Location::next_location,
                     &Location::client_body_bufsize,
                     &Location::parse_index,
                     &Location::parse_autoindex};
    std::cout << line_number << std::endl;
    (this->*word_parse[j])(str);
}

void Location::client_body_bufsize(std::string str){
    size_t i = 0;
    while (str[i] == ' ' || str[i] == '\t')
    {
        i++;
    }
    std::string word;
    word = "client_max_body_size";
    i += word.length() + 1;
    word = "";
    while (str[i] == ' ' && i < str.length())
        i++;
    while (i < str.length() && isdigit(str[i])){
        word+=str[i];
        i++;
    }
    if (i != str.length())
        throw ServerBlock::BadConfig();
    this->clientBodyBufferSize = atoi((const char *)word.c_str());
    std::cout << this->clientBodyBufferSize << std::endl;
}

void Location::cgi_pass(std::string str){
    size_t i = 0;
    while ((str[i] == ' ' || str[i] == '\t') && i < str.length())
        i++;
    std::string word;
    word = "cgi_pass";
    i += word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    word = "";
    while (i < str.length()){
        word += str[i];
        i++;
    }
    this->_cgi_pass = word;
    std::cout << this->_cgi_pass << std::endl;
}


void Location::next_location(std::string str){
    this->next_loc = true;
    // Location *loc = newLocation(this->autoindex);
    Location loc(this->autoindex);
    // loc->ParseLocation(this->filename);
    this->locs.push_back(loc);
    this->locs[this->locs.size() - 1].ParseLocation(this->filename);
}

void Location::root(std::string str){
    size_t i = 0;
    while ((str[i] == ' ' || str[i] == '\t') && i < str.length())
        i++;
    std::string word;
    word = "root";
    i += word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    word = "";
    while (i < str.length()){
        word += str[i];
        i++;
    }
    this->_root = word == "" ? this->_root : word;
    std::cout << this->_root << std::endl;
}

void Location::index(std::string str){
    size_t i = 0;
    while ((str[i] == ' ' || str[i] == '\t') && i < str.length())
        i++;
    std::string word;
    word = "index";
    i += word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    while (i < str.length()){
        word = "";
         while (str[i] != ' ' && i < str.length()){
            word += str[i];
            i++;
        }
        this->indexes.push_back(word);
        while (str[i] == ' ' || str[i] == '\t')
        i++;
    }
    int u = 0;
    while (u < this->indexes.size()){
        std::cout << this->indexes[u] << std::endl;
        u++;
    }
}

void Location::closed_scope(std::string str){
    this->open_scope = false;
    line_number++;
}

size_t Location::ParseLocation(std::string filename){
    this->next_loc = false;
    this->filename = filename;
    std::string buf;
    std::ifstream file(filename);
    if (!file)
        throw ServerBlock::BadConfig();
    size_t n = 0;
    while (n < line_number){
        std::getline(file, buf);
        n++;
    }
    ParseDirects(buf);
    while ( this->open_scope && !this->next_loc)
    {
        std::getline(file, buf);
        line_number++;
        this->ParseTokens(buf);
    }
    return line_number;
}