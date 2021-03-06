/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:27:16 by clorin            #+#    #+#             */
/*   Updated: 2022/03/04 09:33:59 by clorin           ###   ########.fr       */
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
#include "CGIHandler.hpp"
#include "Response.hpp"
#include "Base64.hpp"

#define NO_SOCKET -1
#define MAX_SIZE 2008000
#define MAX_SIZE_HEADER 8000

class Client
{
private:
    int                     _socket;
    struct sockaddr_in      _address;
    std::string             _client_ipv4_str;
    Request *               _request_in_progress;
    std::vector<Response*>  _responses_to_build;
    std::queue<Response*>   _responses_to_send;
    int                     _current_sending_byte;
    char                    _receiving_buff[MAX_SIZE];
    int                     _current_receiving_byte;
    int                     _server_socket;
    bool                    _headers_read;
    bool                    _body_read;
    bool                    _awaiting_trailer;

    Server * findMatchingServer(std::vector<Server*>, Request & request);
    bool readChunkedRequest(Request *, int, int, int);
    void buildErrorResponse(unsigned int status, Server * server);
    
public:
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

    bool receiveFromClient(std::vector<Server*>);
    bool sendToClient();


    bool     setup(Server *server);
    
};

#endif