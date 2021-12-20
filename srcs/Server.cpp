/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:33:58 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:37:53 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _port(-1), _server_name("") {
    std::cout << "construct server" << std::endl;
}

Server::Server(int port, std::string server_name, int max_body_size) : _port(port), _server_name(server_name), _max_body_size(max_body_size)
{   
}

Server::Server(Server const &cpy)
{
    if (this != &cpy) {
		*this = cpy;
	}
}

Server::~Server(){}

Server      &Server::operator=(Server const &cpy)
{
    if(this != &cpy)
    {
        //
    }
    return *this;
}

int Server::getPort() const { return _port; }

std::string Server::getHost() const { return _host; }

std::string Server::getServerName() const { return _server_name; }

void Server::storeLine(std::string & key, std::string & value)
{
    if (key == "listen")
    {
        if (value.find(':') != std::string::npos)
        {
            _host = value.substr(0, value.find(":"));
            std::istringstream ss(std::string(value.substr(value.find(":") + 1, value.length() - value.find(":") - 1)));
            ss >> _port; 
        }
        else
        {
            _host = "127.0.0.1";
            std::istringstream ss(value);
            ss >> _port; 
        }
    }
    else if (key == "server_name")
        _server_name = value;
    else if (key == "max_body_name")
    {
        std::istringstream ss(value);
        ss >> _max_body_size;
    }
}

bool Server::setup()
{
    _listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_listen_socket < 0) {
        perror("socket");
        return false;
    }
    
    int reuse = 1;
    if (setsockopt(_listen_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
        perror("setsockopt");
        return false;
    }
    
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr(_host.c_str());
    my_addr.sin_port = htons(_port);
    
    if (bind(_listen_socket, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) != 0) {
        perror("bind");
        return false;
    }
    
    // start accept client connections
    if (listen(_listen_socket, 10) != 0) {
        perror("listen");
        return false;
    }
    printf("Accepting connections on port %d.\n", _port);
    return true;
}
