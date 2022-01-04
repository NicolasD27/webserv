/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:27:16 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:48:28 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <queue>
#include <netinet/in.h>
#include <string.h>
#include <fstream>
#include <unistd.h>

class Client;

#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

#define NO_SOCKET -1
#define MAX_SIZE 30000
class Client
{
private:
    int     _socket;
    struct sockaddr_in _address;
    std::queue<std::string> _message_queue;
    char _sending_buff[MAX_SIZE];
    int _current_sending_byte;
    char _receiving_buff[MAX_SIZE];
    int _current_receiving_byte;
    
public:
    Client();
    Client(Client const &);
    virtual ~Client();
    Client &operator=(Client const &);

    int getSocket() const;
    std::string const & getCurrentMessage() const;
    bool hasMessages() const;

    bool receiveFromClient();
    bool sendToClient();


    bool     setup(int listen_socket);
    
};

#endif