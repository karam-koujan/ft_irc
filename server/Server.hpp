/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:05:08 by kkoujan           #+#    #+#             */
/*   Updated: 2026/06/26 09:07:13 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/socket.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>    
#include <poll.h>
#include <netinet/in.h>
#include <vector>
#include "Client.hpp"

#define BUFFER_SIZE 4096


class Server
{
    private:
        std::string                _password;
        int                        _port;
        int                        _listen_fd;
        std::vector<struct pollfd> _fd_list;
        std::map<int, Client>      _clients;

        void handle_new_connection();
        void handle_client_read(size_t index);
        void handle_client_write(size_t index);

    public :
        Server();
        Server(int port, std::string password);       
        Server(const Server& other);
        Server& operator=(const Server& other);
        ~Server();
        
        void start();
};

#endif