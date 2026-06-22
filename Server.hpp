/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:32:09 by kkoujan           #+#    #+#             */
/*   Updated: 2026/06/22 18:58:38 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/socket.h>

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