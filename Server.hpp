/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:32:09 by kkoujan           #+#    #+#             */
/*   Updated: 2026/06/22 18:43:48 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

class Server
{
    private:
        std::string 
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