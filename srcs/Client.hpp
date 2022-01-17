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
// private:
public:
    int     _socket;
    struct sockaddr_in _address;
    std::vector<Response*> _responses_to_build;
    std::queue<Response*> _responses_to_send;
    char _sending_buff[MAX_SIZE];
    int _current_sending_byte;
    char _receiving_buff[MAX_SIZE];
    int _current_receiving_byte;
    Server const *_server;
    
    Client();
    Client(Client const &);
    virtual ~Client();
    Client &operator=(Client const &);

    int getSocket() const;
    Server const & getServer() const;
    const Response * getCurrentSendableResponse() const;
    bool hasResponseToSend() const;
    std::vector<Response*>::iterator getBeginResponseToBuild(); 
    std::vector<Response*>::iterator getEndResponseToBuild();
    size_t getResponseToBuildSize() const;
    void switchToSendQueue(Response* response);

    bool receiveFromClient();
    bool sendToClient();


    bool     setup(Server const & server);
    
};

#endif