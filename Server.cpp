/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:39:32 by kkoujan           #+#    #+#             */
/*   Updated: 2026/06/22 18:42:23 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"Server::Server() {
    std::cout << "Default constructor called" << std::endl;
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
    }
    return *this;
}

Server::~Server() {
}