/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* Server.cpp                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/06/22 18:39:32 by kkoujan           #+#    #+#             */
/* Updated: 2026/06/25 16:40:00 by kkoujan          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "Server.hpp"

// =============================================================================
// CLIENT CLASS IMPLEMENTATION
// =============================================================================

Client::Client() : _fd(-1) {}

Client::Client(int fd) : _fd(fd) {}

Client::Client(const Client& other) { *this = other; }

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        this->_fd = other._fd;
        this->_input_buffer = other._input_buffer;
        this->_output_buffer = other._output_buffer;
    }
    return *this;
}

Client::~Client() {}

int          Client::getFd() const { return _fd; }
std::string& Client::getInputBuffer() { return _input_buffer; }
std::string& Client::getOutputBuffer() { return _output_buffer; }


// =============================================================================
// SERVER CLASS IMPLEMENTATION
// =============================================================================

Server::Server() : _port(0), _listen_fd(-1) {}

Server::Server(int port, std::string password) : _password(password), _port(port), _listen_fd(-1) {}

Server::Server(const Server& other) { *this = other; }

Server& Server::operator=(const Server& other) {
    if (this != &other) {
        this->_port = other._port;
        this->_password = other._password;
        this->_listen_fd = other._listen_fd;
        this->_fd_list = other._fd_list;
        this->_clients = other._clients;
    }
    return *this;
}

Server::~Server() {
    // Ensure all remaining sockets are cleaned up on structural shutdown
    for (size_t i = 0; i < _fd_list.size(); ++i) {
        close(_fd_list[i].fd);
    }
}

void Server::start()
{
    // 1. Create IPv4 TCP Stream Socket
    _listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (_listen_fd < 0) {
        throw std::runtime_error("socket creation failed");
    }

    // 2. Clear port reuse hold time (prevents "Address already in use" errors)
    int sock_opt = 1;
    if (setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(int)) < 0) {
        throw std::runtime_error("setsockopt failed");
    }

    // 3. Bind socket to designated port and listen
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(_listen_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        close(_listen_fd);
        throw std::runtime_error("bind failed");
    }
    if (listen(_listen_fd, 128) < 0) { // Set standard backlog queue to 128
        throw std::runtime_error("listen failed");
    }

    // 4. Force master socket into Non-Blocking mode as strictly demanded [cite: 99, 134]
    int flags = fcntl(_listen_fd, F_GETFL, 0);
    if (flags < 0 || fcntl(_listen_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        throw std::runtime_error("fcntl non-block setup failed");
    }

    // 5. Register the master socket to monitor incoming connections (POLLIN)
    struct pollfd listen_fd_struct;
    listen_fd_struct.fd = _listen_fd;
    listen_fd_struct.events = POLLIN;
    listen_fd_struct.revents = 0;
    _fd_list.push_back(listen_fd_struct);

    std::cout << "IRC Server started on port " << _port << "..." << std::endl;

    // 6. Central Non-Blocking I/O loop
    while (true)
    {
        int p = poll(_fd_list.data(), _fd_list.size(), -1);
        if (p < 0) {
            throw std::runtime_error("poll failed");
        }

        for (size_t i = 0; i < _fd_list.size(); i++)
        {
            // Handle Incoming Data / Read Event
            if (_fd_list[i].revents & POLLIN)
            {
                if (_fd_list[i].fd == _listen_fd) {
                    handle_new_connection();
                } else {
                    handle_client_read(i);
                }
            }
            
            // Handle Outgoing Data / Write Event (Only triggers if POLLOUT was deliberately enabled)
            if (_fd_list[i].revents & POLLOUT)
            {
                handle_client_write(i);
            }
        }       
    }          
}

void Server::handle_new_connection()
{
    int new_fd = accept(_listen_fd, NULL, NULL);
    if (new_fd < 0) {
        return; // Non-blocking edge case, reject gracefully instead of crashing [cite: 20]
    }

    // Force newly accepted connection into non-blocking mode [cite: 99, 134]
    int flags = fcntl(new_fd, F_GETFL, 0);
    if (flags < 0 || fcntl(new_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(new_fd);
        return;
    }

    // Track file descriptor inside poll tracking list 
    struct pollfd connect_struct;
    connect_struct.fd = new_fd;
    connect_struct.events = POLLIN; // Monitor read availability initially
    connect_struct.revents = 0;
    _fd_list.push_back(connect_struct);

    // Instantiate client data model mapping tracking context to fd
    _clients[new_fd] = Client(new_fd);
    std::cout << "New client connected on fd: " << new_fd << std::endl;
}

void Server::handle_client_read(size_t index)
{
    char buffer[BUFFER_SIZE];
    int client_fd = _fd_list[index].fd;
    
    size_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0)
    {
        // Handle clean disconnections or explicit read failures
        std::cout << "Client on fd " << client_fd << " disconnected." << std::endl;
        close(client_fd);
        _clients.erase(client_fd);
        _fd_list.erase(_fd_list.begin() + index);
        return;
    }

    buffer[bytes] = '\0';
    Client& client = _clients[client_fd];
    client.getInputBuffer() += buffer; // Append raw bytes to client buffer 

    // Packet Reconstruction: Extract isolated complete lines ending with \n 
    size_t newline_pos;
    while ((newline_pos = client.getInputBuffer().find('\n')) != std::string::npos)
    {
        std::string raw_command = client.getInputBuffer().substr(0, newline_pos);
        
        // Strip trailing carriage returns (\r) typical in IRC protocols
        if (!raw_command.empty() && raw_command[raw_command.size() - 1] == '\r') {
            raw_command.erase(raw_command.size() - 1);
        }

        // Process the fully reconstructed command string
        if (!raw_command.empty()) {
            std::cout << "Extracted command from fd " << client_fd << ": " << raw_command << std::endl;
            
            // --- HANDOFF TO TEAMMATES ---
            // Inside your final project architecture, raw_command will be passed to 
            // Person 2's Parser, and the response string will be queued into the 
            // output buffer like below:
            
            client.getOutputBuffer() += "Server Received: " + raw_command + "\r\n";
            
            // Turn on POLLOUT flag to notify poll we have data to write safely 
            _fd_list[index].events |= POLLOUT; 
        }

        // Erase extracted segment from input stream cache
        client.getInputBuffer().erase(0, newline_pos + 1);
    }
}

void Server::handle_client_write(size_t index)
{
    int client_fd = _fd_list[index].fd;
    Client& client = _clients[client_fd];
    std::string& output = client.getOutputBuffer();

    if (output.empty()) {
        // Stop monitoring writable status if nothing is queued to prevent busy loop overhead [cite: 103]
        _fd_list[index].events &= ~POLLOUT;
        return;
    }

    // Send whatever fits inside the non-blocking kernel buffer out safely
    int bytes_sent = send(client_fd, output.c_str(), output.size(), 0);
    if (bytes_sent < 0) {
        return; // Try again on next safe loop iteration
    }

    // Remove successfully delivered bytes from memory queue
    output.erase(0, bytes_sent);

    // If completely cleared, disable write notifications until Person 3 queues a new message
    if (output.empty()) {
        _fd_list[index].events &= ~POLLOUT;
    }
}