/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:32:09 by kkoujan           #+#    #+#             */
/*   Updated: 2026/06/23 20:26:47 by kkoujan          ###   ########.fr       */
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

class Server
{
    private:
        std::string _password;
        int _port;
        int _listen_fd;
    public :
        // Default Constructor
        Server();
        Server(int port, std::string password);       
        // Copy Constructor
        Server(const Server& other);
        
        // Copy Assignment Operator
        Server& operator=(const Server& other);
        // Destructor
        ~Server();
        void    start();
};



#endif