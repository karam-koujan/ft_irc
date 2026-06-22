/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:39:32 by kkoujan           #+#    #+#             */
/*   Updated: 2026/06/22 20:09:36 by kkoujan          ###   ########.fr       */
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


Server::start()
{
    _listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (_listen_fd < 0)
    {
        std::runtime_error("socket is failed");
    }
    int sock_opt = 1;
    if (setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(int)) < 0)
    {
        std::runtime_error("setsockeopt is failed");
    }
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(_listen_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::runtime_error("bind socket is failed");
    }
    if (listen(_listen_fd, 10)  < 0)
    {
        std::runtime_error("listen is failed");  
    }
}