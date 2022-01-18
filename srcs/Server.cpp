/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:33:58 by clorin            #+#    #+#             */
/*   Updated: 2022/01/17 10:09:38 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _port(-1), _server_name("") {
}

Server::Server(int port, std::string server_name, int max_body_size) : _port(port), _server_name(server_name), _max_body_size(max_body_size), _root("/")
{   
}

Server::Server(Server const &cpy)
{
    if (this != &cpy) {
		*this = cpy;
	}
}

Server::~Server()
{
    for (iterator it = _clients.begin(); it != _clients.end(); ++it)
        delete (*it);
    _clients.clear();
    _error_pages.clear();
    _locations.clear();
}

Server      &Server::operator=(Server const &cpy)
{
    if(this != &cpy)
    {
        _port = cpy._port;
        _host = cpy._host;
        _server_name = cpy._server_name;
        _root = cpy._root;
        _listen_socket = cpy._listen_socket;
        _clients = cpy._clients;
        _max_body_size = cpy._max_body_size;
    }
    return *this;
}

int Server::getPort() const { return _port; }

int Server::getSocket() const { return _listen_socket; }

std::string Server::getHost() const { return _host; }

std::string Server::getRoot() const { return _root; }

std::vector<std::string>    Server::getIndex() const { return _index;}

std::string Server::getServerName() const { return _server_name; }

Client* Server::getClient(int i) const { return _clients[i]; }

std::vector<Location>  Server::getLocation(void)const {return _locations;}

std::vector<std::string>    Server::getMethods(void) const {return _methods;}

std::vector<Client*>::const_iterator Server::getBeginClients() const { return _clients.cbegin(); }

std::vector<Client*>::const_iterator Server::getEndClients() const { return _clients.cend(); }

std::map<std::vector<unsigned int>, std::string> Server::getErrorPages() const { return _error_pages;}

bool        Server::getAutoIndex() const { return _auto_index; }

void Server::storeLine(std::string & key, std::string & value)
{
    //_root = "";
    //_index = "index.html";
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
    else if (key == "root")
        _root = value;
    else if (key == "autoindex")
        _auto_index = (value == "on");
    else if (key == "error_page")
        parseErrorPages(value);
    else if (key == "max_body_size")
    {
        std::istringstream ss(value);
        ss >> _max_body_size;
    }
}
void Server::parseErrorPages(std::string & value)
{
    unsigned int page_number;
    std::vector<unsigned int> pages_numbers;
    std::stringstream iss_line(value);
    std::string pages_numbers_string =value.substr(0, value.find(":"));
    std::string error_page = std::string(value.substr(value.find(":") + 1, value.length() - value.find(":") - 1));
    
    while (std::getline(iss_line, pages_numbers_string, ','))
    {
        std::stringstream ss(pages_numbers_string.substr(0, pages_numbers_string.find(",")));
        ss >> page_number; 
        pages_numbers.push_back(page_number);
        pages_numbers_string = pages_numbers_string.substr(pages_numbers_string.find(",") + 1, pages_numbers_string.length() - pages_numbers_string.find(",") - 1);
    }
    _error_pages.insert(std::make_pair(pages_numbers, error_page));
    // for (std::map<std::vector<unsigned int>, std::string>::iterator it = _error_pages.begin(); it != _error_pages.end(); ++it)
    //     for (std::vector<unsigned int>::const_iterator ite = it->first.begin(); ite != it->first.end(); ++ite)
    //         std::cout << *ite << " : " << it->second << std::endl;
}
bool Server::setup()
{
    _listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (_listen_socket < 0)
        return false;
     int reuse = 1;
    if (setsockopt(_listen_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0)
        return false;
     struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr(_host.c_str());
    my_addr.sin_port = htons(_port);
    std::cout << my_addr.sin_port << std::endl;
    if (bind(_listen_socket, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) != 0)
        return false;
         // start accept client connections
    if (listen(_listen_socket, 10) != 0)
        return false;
    printf("Accepting connections on port %d.\n", _port);
    return true;
}

void Server::handleNewConnection()
{
    Client *new_client = new Client();
    new_client->setup(*this);
    _clients.push_back(new_client);
}

void    Server::addLocation(Location &loc)
{
    _locations.push_back(loc);
}

void    Server::addIndex(std::vector<std::string> &index)
{
    if(!_index.empty())
        _index.clear();
    _index.assign(index.begin() + 1, index.end());
}

void    Server::addMethods(std::string &method)
{
    _methods.push_back(method);
}

void    Server::setMethods(std::vector<std::string> &methods)
{
    if(!_methods.empty())
        _methods.clear();
    _methods.assign(methods.begin(), methods.end());
}

void Server::print(void)const
{
    std::cout << "host : " << _host << ":" << _port << std::endl;
    std::cout << "server_name : " << _server_name << std::endl;
    std::cout << "root : " << _root << std::endl;
    std::cout << "Client Max Body Size : " << _max_body_size << std::endl;
    for(int i = 0; i < _locations.size(); i++)
        _locations[i].print();
    std::cout << std::endl;
}

const char	*Server::FailedSetup::what() const throw(){
	return "Failed to setup server";
}

