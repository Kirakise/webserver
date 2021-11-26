#include "Cluster.hpp"
#include <sys/select.h>
#include <iostream>

Cluster::Cluster(std::vector <ServerConf> &d){
    for (int i = 0; i < d.size(); i++){
        Server s(d[i]);
        _servers.push_back(s);
    }
}

Cluster::~Cluster() {}

int Cluster::setup(){
    FD_ZERO(&_fd_set);
    for (int i = 0; i < _servers.size(); i++)
    {
        _servers[i].setServer();
        FD_SET(_servers[i].getFD(), &_fd_set);
        if (_fd_max < _servers[i].getFD())
            _fd_max = _servers[i].getFD();
    }
    return (1);
}
    
void Cluster::run() {
    while (1)
    {
        bool b = false;
        fd_set wr_set;
        fd_set rd_set;
        struct timeval time;
        int ret = 0;

        while (ret == 0)
        {
            time.tv_sec = 1;
            time.tv_usec = 0;
            memcpy(&rd_set, &_fd_set, sizeof(_fd_set));
            FD_ZERO(&wr_set);
            for (int i = 0; i < _answer.size(); i++)
                FD_SET(_answer[i], &wr_set);
            ret = select(_fd_max + 1, &rd_set, &wr_set, NULL, &time);
        }

        if (ret > 0){
            for (int i = 0; i < _servers.size(); i++)
            {
                if (FD_ISSET(_servers[i].getFD(), &rd_set))
                {
                    uint64_t fd = _servers[i].accept();
                    if (fd > 0)
                    {
                        b = true;
                        FD_SET(fd, &rd_set);
                        _serverMap.insert(std::make_pair(fd, &_servers[i]));
                    }
                }
                if (b)
                    continue;
                    
            }
            s2:
            for (std::unordered_map<uint64_t, Server *>::iterator it = _serverMap.begin();
                it != _serverMap.end(); it++)
                {
                    if (FD_ISSET(it->first, &rd_set))
                    {
                        int ret = it->second->recv(it->first);
                        if (ret == 0)
                        {
                            it->second->doRecv(it->first);
                            _answer.push_back(it->first);
                            FD_SET(it->first, &wr_set);
                        }
                        else if (ret == -1)
                        {
                            _serverMap.erase(it->first);
                            FD_CLR(it->first, &rd_set);
                            goto s2;
                        }
                    }
                }
            s1:
            for (int i = 0; i < _answer.size(); i++)
            {
                if (FD_ISSET(_answer[i], &wr_set))
                {
                    int ret = _serverMap[_answer[i]]->send(_answer[i]);

                    if (ret == 0)
                    {
                        FD_CLR(_answer[i], &wr_set);
                        _answer.erase(_answer.begin() + i);
                    }
                    else if (ret == -1)
                    {
                        FD_CLR(_answer[i], &wr_set);
                        FD_CLR(_answer[i], &rd_set);
                        _serverMap.erase(_answer[i]);
                        _answer.erase(_answer.begin() + i);
                        goto s1;
                    }
                }
            }
        }
        else{
            std::cerr << "Select error!" << std::endl;
            this->clear();
        }
    }
}

void Cluster::clear() {}