/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* Server.hpp                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/06/22 18:32:09 by kkoujan           #+#    #+#             */
/* Updated: 2026/06/25 16:35:00 by kkoujan          ###   ########.fr       */
/* */
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
#include <map> // Added for efficient Client object lookup using file descriptors

#define BUFFER_SIZE 4096

// Client Class: Manages network buffers for each connection to solve partial data issues.
class Client
{
    private:
        int         _fd;
        std::string _input_buffer;  // Assembles raw bytes into complete lines (\n)
        std::string _output_buffer; // Queues outgoing messages safely

    public:
        Client();
        Client(int fd);
        Client(const Client& other);
        Client& operator=(const Client& other);
        ~Client();

        int          getFd() const;
        std::string& getInputBuffer();
        std::string& getOutputBuffer();
};

class Server
{
    private:
        std::string                _password;
        int                        _port;
        int                        _listen_fd;
        std::vector<struct pollfd> _fd_list; // Vector used to store tracked file descriptors dynamically
        std::map<int, Client>      _clients; // Maps active file descriptors directly to their Client data objects

        // Internal sub-routines to keep the main poll loop organized and readable
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