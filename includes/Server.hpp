#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <set>
#include <iostream>


class Server{
    private:
    Server(){}

    void readall(char buf[1024])
    {

    }
    protected:
    public:
    Server(int port) :  port(port) {
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            throw FailedToCreateSocketErr();
        fcntl(sockfd, F_SETFL, O_NONBLOCK);
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = INADDR_ANY;
        sockaddr.sin_port = htons(port);
        if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
            throw FailedToBindConnectionErr();
    }

    int run()
    {
        int connection;
        std::set<int> clients;
        clients.clear();
        if (listen(sockfd, 100) < 0)
            throw FailedToListenOnSocketErr();
        while (1)
        {
            fd_set readset;
            size_t bytes_read;
            FD_ZERO(&readset);
            FD_SET(sockfd, &readset);
            size_t addrlen = sizeof(sockaddr);
            for(std::set<int>::iterator it = clients.begin(); it != clients.end(); it++)
                FD_SET(*it, &readset);
            timeval timeout;
            timeout.tv_sec = 15;
            timeout.tv_usec = 0;
            std::set<int>::iterator it = std::max_element(clients.begin(), clients.end());
            int mx;
            if (it != clients.end())
                mx = std::max(sockfd, *it);
            else
                mx = sockfd;
            if(select(mx+1, &readset, NULL, NULL, &timeout) <= 0)
                throw FailedInSelectErr();
            if(FD_ISSET(sockfd, &readset))
            {
                int sock;
                if((sock = accept(sockfd, NULL, NULL)) < 0)
                    throw FailedToGrabConnectionErr();
            
                fcntl(sock, F_SETFL, O_NONBLOCK);
                clients.insert(sock);
            }
            for(std::set<int>::iterator it = clients.begin(); it != clients.end(); it++)
            {
                if(FD_ISSET(*it, &readset))
                {
                // Поступили данные от клиента, читаем их
                    bytes_read = recv(*it, &buffer, 1024, 0);

                    if(bytes_read <= 0) // Если минус соединение
                    {
                        close(*it);
                        clients.erase(*it);
                        continue;
                    }
                    std::cout << buffer;
                // Тут парсить надо
                // Ответ клиенту
                    send(*it, example.c_str(), example.length(), 0);
                }
            }
        }
        return (0);
    }

    struct FailedToCreateSocketErr : public std::exception
    {
        const char * what() const throw() { return "Failed to create socket"; }
    };

    struct FailedToListenOnSocketErr : public std::exception {
        const char * what() const throw() { return "Failed to listen socket"; }
    };

    struct FailedToGrabConnectionErr : public std::exception {
        const char * what() const throw() { return "Failed accept connection"; }
    };

    struct FailedToBindConnectionErr : public std::exception {
        const char * what() const throw() { return "Failed to bind"; }
    };

    struct FailedInSelectErr : public std::exception {
        const char * what() const throw()  { return "An error poped out during selection"; }
    };

    int sockfd;
    int port;
    sockaddr_in sockaddr;
    size_t addrlen;
    char buffer[1024];
};