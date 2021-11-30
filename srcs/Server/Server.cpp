#include "Server.hpp"
#include "Parser.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void foo(){}

// Server::Server(int port, std::string host) : _port(port) {
//     inet_pton(AF_INET, host.c_str(), &_host);
// }

Server::~Server() {}

Server::Server(ServerConf c) : _port(c.getPort()), _host(inet_addr(c.getHost().c_str())), _conf(c) {}

int Server::setServer()
{
    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = htonl(_host);
    _addr.sin_port = htons(_port);
    if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
    {
        std::cerr << "Error binding port" << _port << std::endl;
        return -1;
    }
    if (listen(_fd, 20) == -1)
    {
        std::cerr << "Could not start listening" << std::endl;
        return -1;
    }
    return 1;
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

	_requests[socket] = head + "\r\n\r\n" + body;
}

int Server::recv(uint64_t socket)
{
    char buf[READ_SIZE];
    int ret;

    ret = ::recv(socket, &buf, READ_SIZE - 1, 0);
    buf[ret] = 0;
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
    size_t pos = 0;
    std::string bound("");

    if(i != std::string::npos)
    {
        if ((pos = _requests[socket].find("Content-Type: multipart/form-data; boundary=")) != std::string::npos)
        {
            bound = _requests[socket].substr(pos);
            pos = bound.find("\r\n") - 45;
            bound = "---" + bound.substr(45, pos) + "--";
            if (_requests[socket].find(bound) == std::string::npos)
                return (1);
            return (0);
        }
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
        size_t len = std::atoi(_requests[socket].substr(_requests[socket].find("Content-length: ") + 16, 10).c_str());
        if (_requests[socket].size() >= len + i + 4)
            return (0);
        else
            return (1);
    }
    return (1);
}


int Server::send(uint64_t socket)
{
    size_t sent;
    while (true)
    {
    std::string sendstr = _requests[socket].substr(0, READ_SIZE);
    if ((sent = ::send(socket, sendstr.c_str(), sendstr.size(), 0)) == -1)
    {
        this->closeSocket(socket);
        return (-1);
    }
    else
    {
        _requests[socket].erase(0, sent);
        if (_requests[socket].size() == 0)
            return 0;
    }
    }
}

void Server::parseMultipart(uint64_t socket)
{
    std::string headers = "";
    std::string body = "";
    std::string bound = "";
    size_t pos;
    pos = _requests[socket].find("Content-Type: multipart/form-data; boundary=");
    headers = _requests[socket].substr(0, pos);
    bound = _requests[socket].substr(pos + 45);
    pos = bound.find("\r\n");
    bound = "---" + bound.substr(0, pos);
    pos = _requests[socket].find(bound + "\r\n");
    body = _requests[socket].substr(pos + bound.size() + 2);
    pos = body.find("\r\n\r\n");
    headers += body.substr(0, pos);
    body = body.substr(pos + 4);
    pos = body.find("\r\n" + bound + "--");
    body = body.substr(0, pos);
    _requests[socket] = headers + "\r\n\r\n" + body;
}

void Server::doRecv(uint64_t socket)
{
    if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos &&
        _requests[socket].find("Transfer-Encoding: chunked") < _requests[socket].find("\r\n\r\n"))
        this->getAllChunks(socket);
    
    if (_requests[socket].find("Content-Type: multipart/form-data; boundary="))
        this->parseMultipart(socket);
    
    if (_requests[socket] != "")
    {
        Parser pars(_requests[socket], *_conf.parent);
        Response p(Parser(pars), _conf.parent->servers[pars.server_index]);
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