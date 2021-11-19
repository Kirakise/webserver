
#include "ServerBlock.hpp"
#include "ServerConf.hpp"

void ServerBlock::ParseAll(){
    try{
        SplitServers();
        ParseServers();
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
    i = word.length();
    while (str[i] == ' ' && i < str.length())
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

}

void ServerBlock::root(std::string str, size_t n)throw (BadConfig){

}
void ServerBlock::index(std::string str, size_t n)throw (BadConfig){

}
void ServerBlock::allow_methods(std::string str, size_t n)throw (BadConfig){

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
    if (str == "")
        return ;
    while (str[i] != ' ' && i < str.length())
        word+=str[i];
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

void ServerBlock::ParseServers() throw (BadConfig)
{
    for(int i = 0; i < this->serverCount; i++)
    {
        this->servers[i].ParseAll();
    }
}

int main()
{
    ServerBlock sv;
    sv.file = "ex.conf";
    sv.ParseAll();
}