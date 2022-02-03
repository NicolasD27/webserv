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
    _request_in_progress = NULL;
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

bool Client::setup(Server * server)
{
    _server = server;
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
    fcntl(_socket, F_SETFL, O_NONBLOCK);
    address = address;
    _current_sending_byte   = -1;
    _current_receiving_byte = 0;
    return true;
}

bool Client::receiveFromClient(std::vector<Server*> servers, int max_body_size)
{
    int r;
    int content_length;
    int newline_pos = -1;
    int prev_newline_pos;
    int offset_newline = 1;
    int current_chunk_size;
    int read_bytes = 0;
    int current_last_nl;
    Request *request;
    std::string request_string;

    if (!_request_in_progress)
    {
        while ((newline_pos == -1 || newline_pos == std::string::npos) && (r = read(_socket, _receiving_buff + _current_receiving_byte, 1)) > 0)
        {
            _current_receiving_byte += r;
            _receiving_buff[_current_receiving_byte] = 0;
            newline_pos = std::string(_receiving_buff).find("\r\n\r\n");
        }
        request_string = std::string(_receiving_buff);
        request = new Request(request_string);
        request->parseHeaders();
        offset_newline = 3;
    }
    else
    {
        request = _request_in_progress;
        newline_pos = _current_receiving_byte - 2;
    }
    if (!((*request)["Transfer-Encoding"].length() == 0 || (*request)["Transfer-Encoding"] == "identity"))
    {
        while (1)
        {
            read_bytes = 0;
            current_last_nl = newline_pos + offset_newline;
            while (newline_pos + offset_newline == current_last_nl && (r = read(_socket, _receiving_buff + _current_receiving_byte, 1)) > 0)
            {
                _current_receiving_byte += r;
                _receiving_buff[_current_receiving_byte] = 0;
                current_last_nl = std::string(_receiving_buff).find_last_of("\r\n");
            }
            
            request_string = std::string(_receiving_buff);
            current_chunk_size = StringHexaToInt(request_string.substr(newline_pos + offset_newline + 1));            
            if (current_chunk_size == 0 || (read_bytes > max_body_size && max_body_size != -1)) // ajouter max body 
                break;
            offset_newline = 1;
            while (read_bytes < current_chunk_size + 2 && (r = read(_socket, _receiving_buff + _current_receiving_byte, current_chunk_size + 2 - read_bytes)) > 0)
            {
                read_bytes += r;
                _current_receiving_byte += r;
                _receiving_buff[_current_receiving_byte] = 0;
            }
            request->addToBody(std::string(_receiving_buff).substr(_current_receiving_byte - current_chunk_size - 1, current_chunk_size));
            newline_pos = std::string(_receiving_buff).find_last_of("\r\n");    
        }        
    }
    else if ((content_length = StringToInt((*request)["Content-Length"])) != 0 && content_length < MAX_SIZE)
    {
        while (read_bytes < content_length && (r = recv(_socket, _receiving_buff + _current_receiving_byte, content_length - read_bytes, MSG_DONTWAIT)) > 0)
        {
            read_bytes += r;
            _current_receiving_byte += r;
            _receiving_buff[_current_receiving_byte] = 0;
        }
        request->addToBody(std::string(_receiving_buff).substr(newline_pos + 4));
    }
    if (request_string.length() == 0)
            return false;
    if (!((*request)["Transfer-Encoding"].length() == 0 || (*request)["Transfer-Encoding"] == "identity") && request_string[(newline_pos + offset_newline + 1)] == 0)
    {
        _request_in_progress = request;
        std::cout << *request << std::endl;
        std::cout << "request chunked" << std::endl;
        
    }
    else
    {
        if (!((*request)["Transfer-Encoding"].length() == 0 || (*request)["Transfer-Encoding"] == "identity"))
        {
            read(_socket, _receiving_buff + _current_receiving_byte, 3); // read the last /r/n
        }
        std::cout << "request finished" << std::endl;
        
            _request_in_progress = NULL;
            _current_receiving_byte = 0;
            _receiving_buff[0] = 0;
        findMatchingServer(servers, *request);
        std::cout << request << std::endl;
        Response* response = new Response(*request, *_server);
        if (response->isToSend())
            _responses_to_send.push(response);
        else
            _responses_to_build.push_back(response);
    }
    return true;
} 

void Client::findMatchingServer(std::vector<Server*> servers, Request & request)
{
    if (request["Host"].length() == 0)
        return;
    for (std::vector<Server*>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        if ((*it)->getSocket() == _server->getSocket() )
        {
            for (std::vector<std::string>::iterator ite = (*it)->getBeginServerNames(); ite != (*it)->getEndServerNames(); ++ite)
            {
                
                if (*ite == request["Host"])
                {
                    _server = *it;
                    if (request.getLocation().substr(0, 7) == "http://")
                        request.setLocation(request.getLocation().substr(7 + request["Host"].length()));
                    return;
                }
            }
        }
    }
}

bool Client::sendToClient()
{
    Response* response = _responses_to_send.front();
    std::cout << *response << std::endl;
    std::string response_string = response->buildResponseString();
    int r = write(_socket, response_string.c_str(), response_string.length());
    if (r == response_string.length())
        _responses_to_send.pop();
    std::cout << "response send" << std::endl;
    return true;
}
