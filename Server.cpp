/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:39:32 by kkoujan           #+#    #+#             */
/*   Updated: 2026/06/23 21:52:48 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {
}

Server::Server(int port, std::string password) {
    this->_port = port; 
    this->_password = password;

}

// Copy Constructor
Server::Server(const Server& other) {
    std::cout << "Copy constructor called" << std::endl;
    *this = other; 
}

// Copy Assignment Operator
Server& Server::operator=(const Server& other) {
    std::cout << "Copy assignment operator called" << std::endl;
    if (this != &other) {
        this->_port = other.port
        this->_password = other.password;
    }
    return *this;
}

Server::~Server() {
}


void Server::start()
{
    _listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (_listen_fd < 0)
    {
        throw std::runtime_error("socket is failed");
    }
    int sock_opt = 1;
    if (setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(int)) < 0)
    {
        throw std::runtime_error("setsockeopt is failed");
    }
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(_listen_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        throw std::runtime_error("bind socket is failed");
        close(_listen_fd);
    }
    if (listen(_listen_fd, 10) < 0)
    {
        throw std::runtime_error("listen is failed");
    }
    int flags = fcntl(_listen_fd, F_GETFL, 0);
    if (flags < 0) {
        throw std::runtime_error("fcntl is failed");
    }
    if (fcntl(_listen_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        throw std::runtime_error("fctnl is failed");
    }
    std::vector<struct pollfd> fd_list;
    struct pollfd listen_fd_struct;
    listen_fd_struct.fd = _listen_fd;
    listen_fd_struct.events = POLL_IN;
    fd_list.push_back(listen_fd_struct);
    char buffer[BUFFER_SIZE];
    while (true)
    {
        int p =  poll(fd_list.data(), fd_list.size(), -1);
        if (p == 0)
        {
            throw std::runtime_error("timeout");
        }else if (p < 0)
        {
            throw std::runtime_error("poll failed");
        }
        for (size_t i = 0; i < fd_list.size(); i++)
        {
            if (fd_list[i].revents & POLL_IN)
            {
                if (fd_list[i].fd == _listen_fd)
                {
                    int new_fd = accept(_listen_fd, nullptr, nullptr);
                    if (new_fd < 0)
                    {
                        throw std::runtime_error("accept failed");
                    }
                    struct pollfd connect_struct;
                    connect_struct.fd = new_fd;
                    connect_struct.events = POLL_IN;
                    fd_list.push_back(connect_struct);
                }
                     
            }
        }       
   
    }          
    }
