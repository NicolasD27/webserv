/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:33:58 by clorin            #+#    #+#             */
/*   Updated: 2022/01/05 08:37:39 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    _socket = NO_SOCKET;
    std::queue<std::string> _message_queue; 

}

Client::Client(Client const &cpy)
{
    if (this != &cpy) {
		*this = cpy;
	}
}

Client::~Client() {}

Client      &Client::operator=(Client const &cpy)
{
    if(this != &cpy)
    {
        _socket = cpy._socket;
        _address = cpy._address;
        _current_sending_byte   = cpy._current_sending_byte;
        _current_receiving_byte = cpy._current_receiving_byte;

    }
    return *this;
}

int Client::getSocket() const { return _socket; }

Server const & Client::getServer() const { return *_server; }


std::string const & Client::getCurrentMessage() const { return _message_queue.front();}

bool Client::hasMessages() const { return _message_queue.size() != 0; }

bool Client::setup(Server const & server)
{
    _server = &server;
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    socklen_t client_len = sizeof(address);
    int new_client_sock = accept(_server->getSocket(), (struct sockaddr *)&address, &client_len);
    if (new_client_sock < 0)
        return false;
    char client_ipv4_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, client_ipv4_str, INET_ADDRSTRLEN);
    
    printf("Incoming connection from %s:%d.\n", client_ipv4_str, address.sin_port);
    _socket = new_client_sock;
    address = address;
    _current_sending_byte   = -1;
    _current_receiving_byte = 0;
    return true;
}

bool Client::receiveFromClient()
{
    char buff[MAX_SIZE];
    int r = read(_socket, buff, MAX_SIZE);
    buff[r] = 0;
    if (r >= 0)
    {

        std::string request_string(buff);
        Request request(request_string);
        request.parseHeaders();
        Response response(request, *_server);
        _message_queue.push(response.getResponseString());
        std::cout << request << std::endl;
        std::cout << "message received" << std::endl;
        std::cout << response << std::endl;
    }
    else
        std::cout << "nothing received" << std::endl;
    return true;
} 

bool Client::sendToClient()
{
    std::cout << "begin send" << std::endl;
    std::string response = _message_queue.front();
    int r = write(_socket, response.c_str(), response.length());
    if (r == response.length())
        _message_queue.pop();
    std::cout << "response send" << std::endl;
    return true;
}
