/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 08:59:23 by kkoujan           #+#    #+#             */
/*   Updated: 2026/06/26 09:02:02 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <map>
#include <string>

class Client
{
    private:
        int         _fd;
        std::string _input_buffer; 
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


#endif