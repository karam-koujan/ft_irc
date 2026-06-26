/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:03:41 by kkoujan           #+#    #+#             */
/*   Updated: 2026/06/26 09:04:21 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


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
