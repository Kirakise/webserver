
#include "ServerBlock.hpp"

extern size_t line_number;

ServerBlock::ServerBlock(){};
ServerBlock::~ServerBlock(){};

void ServerBlock::ParseAll(){
    try{
        SplitServers();
    }
    catch(std::exception const & ex){
        this->badConfig = true;
    }
}

ServerConf *ServerBlock::NewServer(){
    ServerConf *server = new ServerConf();
    return server;
}

void ServerBlock::SplitServers() throw (BadConfig)
{
    size_t n = 0;
    this->flag = false;
    this->serverCount = 0;
    std::string buf;
    std::ifstream file(this->file);
    if (!file)
        throw ServerBlock::BadConfig();
    while(std::getline(file, buf))
    {
        line_number += 1;
        n++;
        if (buf == "server {")
        {
            this->servers.push_back(*this->NewServer());
            this->serverCount++;
            this->servers[serverCount - 1].open_scope = true;
        }
        std::string tmp;
        while(this->servers[serverCount - 1].open_scope)
        {
            std::getline(file, buf);
            n++;
            line_number += 1;
            while (n < line_number){
                tmp = buf;
                std::getline(file, buf);
                n++;
            }
            if (buf == "" && tmp == "}")
                buf = tmp;
            // line_number += 1;
            // n++;
            this->ParseTokens(buf, this->serverCount-1);
            // if (flag)
            //     this->
        }
        size_t q = 0;
    }
}

void ServerBlock::listen(std::string str, size_t n)throw (BadConfig){
    std::string word;
    word = "listen";
    size_t i = 0;
    i = word.length() + 1;
    while ((str[i] == ' ' || str[i] == '\t') && i < str.length())
        i++;
    if (i == str.length())
        throw ServerBlock::BadConfig();
    word = "";
    while (isdigit(str[i]) || str[i] == '.'){
        word+=str[i];
        i++;
    }
    if (str[i] == ':')
        i++;
    else
        throw ServerBlock::BadConfig();
    this->servers[n].host = word;
    word = "";
    while (i < str.length() && i != ' ' && isdigit(str[i]))
    {
        word+=str[i];
        i++;
    }
    if (i != str.length())
        throw ServerBlock::BadConfig();
    this->servers[n]._port = atoi((const char *)word.c_str());
    std::cout << this->servers[n]._port << " " << this->servers[n].host << std::endl;
}

void ServerBlock::serverName(std::string str, size_t n)throw (BadConfig){
    std::string word;
    word = "server_name";
    size_t i = word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    while (i < str.length()){
        word = "";
         while (str[i] != ' ' && i < str.length()){
            word += str[i];
            i++;
        }
        this->servers[n]._serverName.push_back(word);
        while (str[i] == ' ' || str[i] == '\t')
        i++;
    }
    int u = 0;
    while (u < this->servers[n]._serverName.size()){
        std::cout << this->servers[n]._serverName[u] << std::endl;
        u++;
    }
        
}

void ServerBlock::root(std::string str, size_t n)throw (BadConfig){
    
    std::string word;
    word = "root";
    size_t i = word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    word = "";
    while (i < str.length()){
        word += str[i];
        i++;
    }
    this->servers[n]._root = word;
    std::cout << this->servers[n]._root << std::endl;
}

void ServerBlock::index(std::string str, size_t n)throw (BadConfig){
    std::string word;
    word = "index";
    size_t i = word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    while (i < str.length()){
        word = "";
         while (str[i] != ' ' && i < str.length()){
            word += str[i];
            i++;
        }
        this->servers[n]._indexes.push_back(word);
        while (str[i] == ' ' || str[i] == '\t')
        i++;
    }
    int u = 0;
    while (u < this->servers[n]._indexes.size()){
        std::cout << this->servers[n]._indexes[u] << std::endl;
        u++;
    }
}

void ServerBlock::allow_methods(std::string str, size_t n)throw (BadConfig){
    std::string word;
    word = "allow_methods";
    size_t i = word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    while (i < str.length()){
        word = "";
         while (str[i] != ' ' && i < str.length()){
            word += str[i];
            i++;
        }
        this->servers[n].allowedMethods.push_back(word);
        while (str[i] == ' ' || str[i] == '\t')
        i++;
    }
    int u = 0;
    while (u < this->servers[n].allowedMethods.size()){
        std::cout << this->servers[n].allowedMethods[u] << std::endl;
        u++;
    }
}

void ServerBlock::parse_autoindex(std::string str, size_t n)throw (BadConfig){
   std::string word;
    word = "autoindex";
    size_t i = word.length() + 1;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    word = "";
    while (i < str.length()){
        word += str[i];
        i++;
    }
    if (i != str.length())
        throw ServerBlock::BadConfig();
    if (word == "on")
        this->servers[n]._autoindex = true;
    else if (word == "off")
        this->servers[n]._autoindex = false;
    else   
        throw ServerBlock::BadConfig();
    std::cout << this->servers[n]._root << std::endl;
}



void ServerBlock::closed_scope(std::string str, size_t n)throw (BadConfig){
    this->servers[n].open_scope = false;
}

void ServerBlock::ParseTokens(std::string str, size_t n) throw (BadConfig)
{
    std::string types[8] = {"listen",
                        "server_name",
                        "root",
                        "index",
                        "allow_methods",
                        "location",
                        "}",
                        "autoindex"};

    size_t i = 0;
    size_t j = 0;
    std::string word;
    if (str == "" || isempty(str))
        return ;
    while (str[i] == '\t')
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
    typedef void(ServerBlock::*Parse)(std::string str, size_t n);
    Parse word_parse[8] = {&ServerBlock::listen,
                     &ServerBlock::serverName, 
                     &ServerBlock::root, 
                     &ServerBlock::index, 
                     &ServerBlock::allow_methods, 
                     &ServerBlock::locations, 
                     &ServerBlock::closed_scope,
                     &ServerBlock::parse_autoindex};
    (this->*word_parse[j])(str, n);
}

 

void ServerBlock::ServerCount() throw (BadConfig)
{
    size_t server_count = 0;
    size_t pos;
    std::string buf;
    std::ifstream file(this->file);
    if (!file){
        throw ServerBlock::BadConfig();
    }
    while(std::getline(file, buf))
    {
        pos = buf.find("server {");
        while (pos != std::string::npos)
        {
            server_count++;
            buf = buf.substr(pos+8);
            pos = buf.find("server {");
        }
    }
    if (server_count == 0)
        throw ServerBlock::BadConfig();
    this->serverCount = server_count;
}



// LOCATION METHODS:

void ServerBlock::locations(std::string str, size_t n)throw (BadConfig){
    Location *newloc = new Location();
    this->servers[n].locs.push_back(*newloc);
    this->servers[n].loc_number+= 1;
    // std::cout << line_number << std::endl;
    this->servers[n].locs[this->servers[n].loc_number - 1].ParseLocation(this->file);
}




// MAIN FOR DEBUG: 

// int main()
// {
//     ServerBlock sv;
//     sv.file = "srcs/Configs/ex.conf";
//     sv.ParseAll();
// }