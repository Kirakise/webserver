#include "Server.hpp"
#include "Parser.hpp"
#include <iostream>
#include <unistd.h>

Server::Server(uint16_t port, uint64_t host) : _port(port), _host(host){}

Server::~Server() {}

Server::Server(ServerConf c) : _port(c.getPort()), _host(c.getHost()), _conf(c) {}

int Server::setServer()
{
    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return ;
    }
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = htonl(_host);
    _addr.sin_port = htonl(_port);
    if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
    {
        std::cerr << "Error binding port" << _port << std::endl;
        return ;
    }
    if (listen(_fd, 20) == -1)
    {
        std::cerr << "Could not start listening" << std::endl;
        return ;
    }
}

int Server::accept(void)
{
    uint64_t socket;

    socket = ::accept(_fd, 0, 0);
    if (socket == -1)
    {
        std::cerr << "Error accepting socket" << std::endl;
        return -1;
    }
    else
    {
        fcntl(socket, F_SETFL, O_NONBLOCK);
        _requests.insert(std::make_pair(socket, ""));
        return socket;
    }
}


void Server::getAllChunks(uint64_t socket) {
    std::string head = _requests[socket].substr(0, _requests[socket].find("\r\n\r\n"));
    std::string chunks = _requests[socket].substr(_requests[socket].find("\r\n\r\n"), _requests[socket].size() - 1);
    std::string	subchunk = chunks.substr(0, 100);
	std::string	body = "";
    int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;

	while (chunksize)
	{
		i = chunks.find("\r\n", i) + 2;
		body += chunks.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = chunks.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}

	_requests[socket] = head + "\r\n\r\n" + body + "\r\n\r\n";
}

int Server::recv(uint64_t socket)
{
    char buf[READ_SIZE];
    int ret;

    ret = ::recv(socket, &buf, READ_SIZE - 1, 0);

    if (ret <= 0)
    {
        closeSocket(socket);
        if (ret == 0)
            std::cout << "Connection closed by client" << std::endl;
        else
            std::cerr << "Error reading from socket" << std::endl;
        return (-1);
    }

    _requests[socket].append(buf);

    size_t i = _requests[socket].find("\r\n\r\n");

    if(i != std::string::npos)
    {
        if (_requests[socket].find("Content-Length: ") == std::string::npos)
        {
            if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos)
            {
                if (_requests[socket].find("0\r\n\r\n") != std::string::npos)
                    return (0);
                else
                    return (1);
            }
            else
                return (0);
        }
        
        size_t len = std::stoi(_requests[socket].substr(_requests[socket].find("Content-length:") + 16));
        if (_requests[socket].size() >= len + i + 4)
            return (0);
        else
            return (1);
    }
    return (1);
}


int Server::send(uint64_t socket)
{
    std::string sendstr = _requests[socket].substr(0, READ_SIZE);
    if (::send(socket, sendstr.c_str(), sendstr.size(), 0) == -1)
    {
        this->closeSocket(socket);
        return (-1);
    }
    else
    {
        _requests[socket].erase(0, READ_SIZE);
        if (_requests[socket].size() == 0)
            return 0;
        else
            return 1;
    }
}


void Server::doRecv(uint64_t socket)
{
    if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos &&
        _requests[socket].find("Transfer-Encoding: chunked") < _requests[socket].find("\r\n\r\n"))
        this->getAllChunks(socket);
    
    if (_requests[socket] != "")
    {
        Response p(Parser(_requests[socket]), _conf);
        p.Execute();
        _requests[socket] = p.getResponse();
    }
}

void Server::closeSocket(uint64_t socket)
{
    if (socket > 0)
        ::close(socket);
    _requests.erase(socket);
}

int64_t Server::getFD() { return _fd; }


void Server::closeServer(void)
{
    if (_fd > 0)
        ::close(_fd);
    _fd = -1;
}