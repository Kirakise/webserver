#include "ServerConf.hpp"


void ServerBlock::FillThatShitPls(){
    try{
       ServerCount();
       MakeServerArray();
       ParseServers();
    }
    catch(std::exception const & ex){
        this->badConfig = true;
    }
}

void ServerBlock::MakeServerArray()
{
    this->servers = new ServerConf[this->serverCount];
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