
#include "ServerBlock.hpp"
#include "ServerConf.hpp"

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

ServerBlock::ServerConf *ServerBlock::NewServer(){
    ServerConf *server = new ServerConf();
    return server;
}

void ServerBlock::SplitServers() throw (BadConfig)
{
    this->serverCount = 0;
    std::string buf;
    bool open_scope;
    std::ifstream file(this->file);
    if (!file)
        throw ServerBlock::BadConfig();
    while(std::getline(file, buf))
    {
        if (buf == "server {")
        {
            this->servers.push_back(*this->NewServer());
            this->serverCount++;
        }
        while(std::getline(file, buf))
            this->ParseTokens(buf, this->serverCount-1);
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
void ServerBlock::locations(std::string str, size_t n)throw (BadConfig){

}
void ServerBlock::open_scope(std::string str, size_t n)throw (BadConfig){

}
void ServerBlock::closed_scope(std::string str, size_t n)throw (BadConfig){

}

void ServerBlock::ParseTokens(std::string str, size_t n) throw (BadConfig)
{
    std::string types[8] = {"listen",
                        "server_name",
                        "root",
                        "index",
                        "allow_methods",
                        "locations",
                        "{",
                        "}"};

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
                     &ServerBlock::open_scope, 
                     &ServerBlock::closed_scope};
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


int main()
{
    ServerBlock sv;
    sv.file = "srcs/Configs/ex.conf";
    sv.ParseAll();
}