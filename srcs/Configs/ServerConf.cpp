#include "ServerConf.hpp"


void ServerConf::FillThatShitPls(){
    try{
       ServerCount();
       MakeServerArray();
       ParseServers();
    }
    catch(std::exception const & ex){
        this->badConfig = true;
    }
}

void ServerConf::MakeServerArray()
{
    this->servers = new ServerBlock[serverCount];
}

void ServerConf::ServerCount() throw (BadConfig)
{
    size_t server_count = 0;
    size_t pos;
    std::string buf;
    std::ifstream file(this->file);
    if (!file){
        throw ServerConf::BadConfig();
    }
    while(std::getline(file, buf))
    {
        pos = buf.find("server");
        while (pos != std::string::npos)
        {
            server_count++;
            buf = buf.substr(pos+6);
            pos = buf.find("server");
        }
    }
    if (server_count == 0)
        throw ServerConf::BadConfig();
    this->serverCount = server_count;
}

