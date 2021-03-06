/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:33:58 by clorin            #+#    #+#             */
/*   Updated: 2022/03/27 17:53:00 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    _headers_read = false;
    _awaiting_trailer = false;
    _request_in_progress = NULL;
    _socket = NO_SOCKET;
    std::vector<Response*>  _responses_to_build;
    std::queue<Response*>   _responses_to_send;
}

Client::Client(Client const &cpy)
{
    if (this != &cpy) {
		*this = cpy;
	}
}

Client::~Client()
{
    _client_ipv4_str.clear();
    for (std::vector<Response *>::iterator it = _responses_to_build.begin(); it != _responses_to_build.end(); ++it)
        delete *it;
    _responses_to_build.clear();
    while (!_responses_to_send.empty())
    {
        delete _responses_to_send.front();
        _responses_to_send.pop();
    }
    close(_socket);
}

Client      &Client::operator=(Client const &cpy)
{
    if(this != &cpy)
    {
        _socket = cpy._socket;
        _address = cpy._address;
        _current_sending_byte   = cpy._current_sending_byte;
        _current_receiving_byte = cpy._current_receiving_byte;
        _client_ipv4_str = cpy._client_ipv4_str;
        _headers_read = cpy._headers_read;
        _body_read = cpy._body_read;
    }
    return *this;
}

int Client::getSocket() const { return _socket; }

// Server const & Client::getServer() const { return *_server; }

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
    _server_socket = server->getSocket();
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    socklen_t client_len = sizeof(address);
    int new_client_sock = accept(_server_socket, (struct sockaddr *)&address, &client_len);
    if (new_client_sock < 0)
        return false;
    char client_ipv4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, client_ipv4, INET_ADDRSTRLEN);
    this->_client_ipv4_str = client_ipv4;
    _socket = new_client_sock;
    printf("Incoming connection from %s:%d to %s:%d socket : %d\n", this->_client_ipv4_str.c_str(), address.sin_port, server->getHost().c_str(), server->getPort(), _socket);
    _address = address;
    _current_sending_byte   = 0;
    _current_receiving_byte = 0;
    return true;
}

bool Client::receiveFromClient(std::vector<Server*> servers)
{
    int r = 0;
    int content_length = 0;
    size_t newline_pos = std::string::npos;
    size_t offset_newline = 1;
    size_t current_last_nl;
    size_t headers_length;
    std::string request_string;
    Response* response;
    
    if (!_headers_read)
    {
        while ((newline_pos == std::string::npos) && (r = read(_socket, _receiving_buff + _current_receiving_byte, 1)) > 0 && _current_receiving_byte < MAX_SIZE_HEADER)
        {
            _current_receiving_byte += r;
            _receiving_buff[_current_receiving_byte] = 0;
            newline_pos = std::string(_receiving_buff).find("\r\n\r\n");
        }
        if (r <= 0)
            return false;
        if (_current_receiving_byte >= MAX_SIZE_HEADER)
        {
            // while ((r = read(_socket, _receiving_buff , MAX_SIZE)) > 0);
            // if (r <= 0)
            //     return false;
            buildErrorResponse(414, servers.front());
            return true;
        }
        _headers_read = true;
        offset_newline = 3;
        request_string = std::string(_receiving_buff, _current_receiving_byte);
        _request_in_progress = new Request(request_string);
        _request_in_progress->parseHeaders("");
        _request_in_progress->setPortClient(_address.sin_port);
        _request_in_progress->setAddressClient(_client_ipv4_str);
    }
    else
        newline_pos = _current_receiving_byte - 2;
    Server *server = findMatchingServer(servers, *_request_in_progress);
    int max_body_size = server->getMaxBodySize();
    headers_length = request_string.find("\r\n\r\n") + 4;
    _body_read = true;
    if (_awaiting_trailer)
    {
        current_last_nl = _current_receiving_byte - 4;
        newline_pos = std::string::npos;
        while (newline_pos == std::string::npos && (r = read(_socket, _receiving_buff + _current_receiving_byte, 1)) > 0 && _current_receiving_byte < MAX_SIZE - MAX_SIZE_HEADER - 4)
        {
            _current_receiving_byte += r;
            _receiving_buff[_current_receiving_byte] = 0;
            newline_pos = std::string(_receiving_buff + current_last_nl).find("\r\n\r\n");
        }
        if (r <= 0)
            return false;
        if (newline_pos == std::string::npos)
            _body_read = false;
        else
        {
            _request_in_progress->parseHeaders(std::string(_receiving_buff + current_last_nl));
            _awaiting_trailer = false;
        }
    }
    else if (_request_in_progress->headerExist("Transfer-Encoding") && !((*_request_in_progress)["Transfer-Encoding"].length() == 0 || (*_request_in_progress)["Transfer-Encoding"] == "identity"))
    {
        if (!readChunkedRequest(_request_in_progress, newline_pos, offset_newline, max_body_size))
            return false;
    }
    else if (_request_in_progress->headerExist("Content-Length") && (content_length = StringToInt((*_request_in_progress)["Content-Length"])) > 0 && (max_body_size == -1 || content_length <= max_body_size) && content_length < MAX_SIZE - MAX_SIZE_HEADER - 4)
    {
        while (_current_receiving_byte - headers_length < static_cast<size_t>(content_length) && (r = read(_socket, _receiving_buff + _current_receiving_byte, content_length - _current_receiving_byte + headers_length)) > 0)
        {
            _current_receiving_byte += r;
            _receiving_buff[_current_receiving_byte] = 0;
        }
        if (r <= 0)
            return false;
        _body_read = (r <= 0) ? false : true;
        _request_in_progress->addToBody(std::string(_receiving_buff, content_length + newline_pos + 4).substr(newline_pos + 4));
    }
    request_string = std::string(_receiving_buff, _current_receiving_byte);
    if (max_body_size != -1 && content_length > max_body_size)
    {
        std::cout << *_request_in_progress << std::endl;
        // while ((r = read(_socket, _receiving_buff , MAX_SIZE)) > 0);
        // if (r <= 0)
        //     return false;
        buildErrorResponse(413, server);
        delete _request_in_progress;
    }
    else if (request_string.length() == 0)
    {
        delete _request_in_progress;
        _headers_read = false;
        _current_receiving_byte = 0;
        _receiving_buff[0] = 0;
        return false;
    }
    else if (_body_read)
    {
        _current_receiving_byte = 0;
        _receiving_buff[0] = 0;
        _headers_read = false;
        std::cout << *_request_in_progress << std::endl;
        response = new Response(*_request_in_progress, *server);
        if (response->isToSend())
            _responses_to_send.push(response);
        else
            _responses_to_build.push_back(response);
    }
    else
    {
        std::cout << _receiving_buff << std::endl;
        std::cout << "request not finished" << std::endl;
    }
    return true;
} 

void Client::buildErrorResponse(unsigned int status, Server * server)
{
    Response *response;

    _current_receiving_byte = 0;
    _receiving_buff[0] = 0;
    _headers_read = false;
    response = new Response(status, *server);
    if (response->isToSend())
        _responses_to_send.push(response);
    else
        _responses_to_build.push_back(response);
    
}



bool Client::readChunkedRequest(Request *request, int newline_pos, int offset_newline, int max_body_size)
{
    int r;
    int current_chunk_size;
    int read_bytes = 0;
    int current_last_nl;
    std::string request_string;

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
            if (r <= 0)
                return false;
            if (r == 0)
                current_last_nl++;
            request_string = std::string(_receiving_buff);
            current_chunk_size = StringHexaToInt(request_string.substr(newline_pos + offset_newline + 1));            
            if (current_chunk_size == 0 || (read_bytes > max_body_size && max_body_size != -1))
            {
                if (request_string[(newline_pos + offset_newline + 1)] == 0)
                    _body_read = false;
                else
                {
                    if ((r = read(_socket, _receiving_buff + _current_receiving_byte, 2)) > 0)
                    {
                        _current_receiving_byte += r;
                        _receiving_buff[_current_receiving_byte] = 0;
                        if (std::string(_receiving_buff + current_last_nl) == "\r\n\r\n")
                            _body_read = true;
                        else
                        {
                            _awaiting_trailer = true;
                            _body_read = false;
                        }
                    }
                    else
                        return false;
                }
                return true;
            }
            offset_newline = 1;
            while (read_bytes < current_chunk_size + 2 && (r = read(_socket, _receiving_buff + _current_receiving_byte, current_chunk_size + 2 - read_bytes)) > 0)
            {
                read_bytes += r;
                _current_receiving_byte += r;
                _receiving_buff[_current_receiving_byte] = 0;
            }
            if (r <= 0)
                return false;
            request->addToBody(std::string(_receiving_buff).substr(_current_receiving_byte - current_chunk_size - 1, current_chunk_size));
            newline_pos = std::string(_receiving_buff).find_last_of("\r\n");    
        }        
}

Server * Client::findMatchingServer(std::vector<Server*> servers, Request & request)
{
    Server * new_server = NULL;
    if (request.getLocation().substr(0, 7) == "http://")
        request.setLocation(request.getLocation().substr(7 + request["Host"].length()));
    for (std::vector<Server*>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        if ((*it)->getSocket() == _server_socket )
        {
            if (!new_server)
                new_server = *it;
            for (std::vector<std::string>::iterator ite = (*it)->getBeginServerNames(); ite != (*it)->getEndServerNames(); ++ite)
            {
                if (*ite == request["Host"] || *ite + ":" + to_string((*it)->getPort()) == request["Host"])
                {
                    _server_socket = (*it)->getSocket();
                    return (*it);
                }
            }
        }
    }
    return new_server;
}



bool Client::sendToClient()
{
    Response* response = _responses_to_send.front();
    std::string response_string = response->buildResponseString();
    std::cout << *response << std::endl;
    int r = write(_socket, response_string.c_str() + _current_sending_byte, response_string.length() - _current_sending_byte);
    if  (r <= 0)
        return false;
    else if (r == static_cast<int>(response_string.length()) - _current_sending_byte)
    {
        std::cout << "response send" << std::endl;
        _current_sending_byte = 0;
        delete response;
        _responses_to_send.pop();
        if (_responses_to_send.size() == 0 && _responses_to_build.size() == 0)
            return false;
    }
    else        
        _current_receiving_byte += r;
    return true;
}
