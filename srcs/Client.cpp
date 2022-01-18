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
    std::queue<std::string> _response_queue; 

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

std::vector<Response*>::iterator Client::getBeginResponseToBuild() { return _responses_to_build.begin(); }
std::vector<Response*>::iterator Client::getEndResponseToBuild() { return _responses_to_build.end(); }

size_t Client::getResponseToBuildSize() const { return _responses_to_build.size(); }
const Response * Client::getCurrentSendableResponse() const { return _responses_to_send.front();}

bool Client::hasResponseToSend() const { return _responses_to_send.size() != 0; }


void Client::switchToSendQueue(Response* response)
{
    std::vector<Response*>::iterator it_to_erase;
    _responses_to_send.push(response);
    for (std::vector<Response*>::iterator ite = _responses_to_build.begin(); ite != _responses_to_build.end(); ++ite)
    {
        if ((*ite)->getRequest() == response->getRequest())
        {
            it_to_erase = ite;
        }
     }
    _responses_to_build.erase(it_to_erase);
}

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
    int r;
    int content_length;
    int newline_pos = -1;
    int prev_newline_pos;
    int offset_newline = 3;
    int current_chunk_size;
    int read_bytes;
    int current_last_nl;

    _current_receiving_byte = 0;
    _receiving_buff[0] = 0;
    while ((newline_pos == -1 || newline_pos == std::string::npos) && (r = recv(_socket, _receiving_buff + _current_receiving_byte, 1, MSG_DONTWAIT)) > 0)
    {
        _current_receiving_byte += r;
        _receiving_buff[_current_receiving_byte] = 0;
        newline_pos = std::string(_receiving_buff).find("\r\n\r\n");
    }
    std::string request_string(_receiving_buff);
    Request request(request_string);
    request.parseHeaders();
    if (!(request["Transfer-Encoding"].length() == 0 || request["Transfer-Encoding"] == "identity"))
    {
        while (1)
        {
            read_bytes = 0;
            current_last_nl = newline_pos + offset_newline;
            while (newline_pos + offset_newline == current_last_nl && (r = recv(_socket, _receiving_buff + _current_receiving_byte, 1, MSG_DONTWAIT)) > 0)
            {
                _current_receiving_byte += r;
                _receiving_buff[_current_receiving_byte] = 0;
                current_last_nl = std::string(_receiving_buff).find_last_of("\r\n");
            }
            offset_newline = 0;
            request_string = std::string(_receiving_buff);
            current_chunk_size = StringHexaToInt(request_string.substr(newline_pos + 1));
            offset_newline = 1;
            if (current_chunk_size == 0) // ajouter max body
                break;
            while (read_bytes < current_chunk_size + 2 && (r = recv(_socket, _receiving_buff + _current_receiving_byte, current_chunk_size + 2 - read_bytes, MSG_DONTWAIT)) > 0)
            {
                read_bytes += r;
                _current_receiving_byte += r;
                _receiving_buff[_current_receiving_byte] = 0;
            }
            request.addToBody(std::string(_receiving_buff).substr(_current_receiving_byte - current_chunk_size - 1, current_chunk_size));
            newline_pos = std::string(_receiving_buff).find_last_of("\r\n");    
        }        
    }
    else if ((content_length = StringToInt(request["Content-Length"])) != 0)
    {
        while (read_bytes < content_length && (r = recv(_socket, _receiving_buff + _current_receiving_byte, content_length - read_bytes, MSG_DONTWAIT)) > 0)
        {
            read_bytes += r;
            _current_receiving_byte += r;
            _receiving_buff[_current_receiving_byte] = 0;
        }
        request.addToBody(std::string(_receiving_buff).substr(newline_pos + 4));
    }
    std::cout << request << std::endl;
    Response* response = new Response(request, *_server);
    if (response->isToSend())
        _responses_to_send.push(response);
    else
        _responses_to_build.push_back(response);
    std::cout << "message received" << std::endl;
    // std::cout << response << std::endl;
    return true;
} 

bool Client::sendToClient()
{
    std::cout << "begin send" << std::endl;
    Response* response = _responses_to_send.front();
    std::string response_string = response->buildResponseString();
    int r = write(_socket, response_string.c_str(), response_string.length());
    if (r == response_string.length())
        _responses_to_send.pop();
    std::cout << "response send" << std::endl;
    return true;
}
