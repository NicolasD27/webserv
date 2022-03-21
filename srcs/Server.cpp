/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:33:58 by clorin            #+#    #+#             */
/*   Updated: 2022/02/21 10:59:29 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


Server::Server() : _host(""), _port(-1), _max_body_size(-1), _is_listening(false) {
}


Server::Server(Server const &cpy)
{
    if (this != &cpy) {
		*this = cpy;
	}
}

Server::~Server()
{
    for (client_iterator it = _clients.begin(); it != _clients.end(); ++it)
        delete (*it);
    for (location_iterator it = _locations.begin(); it != _locations.end(); ++it)
        delete (*it);
}

void Server::removeClient(int socket)
{
    for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->getSocket() == socket)
        {
            delete *it;
            _clients.erase(it);
            return;
        }
    }
}

Server      &Server::operator=(Server const &cpy)
{
    if(this != &cpy)
    {
        _port = cpy._port;
        _host = cpy._host;
        _server_names = cpy._server_names;
        _root = cpy._root;
        _listen_socket = cpy._listen_socket;
        _clients = cpy._clients;
        _max_body_size = cpy._max_body_size;
        _auto_index = cpy._auto_index;
        _error_pages = cpy._error_pages;
        _locations = cpy._locations;
        _methods = cpy._methods;
        _cgi_path = cpy._cgi_path;
        _is_listening = cpy._is_listening;
    }
    return *this;
}

int Server::getNbClients() const { return _clients.size(); }

int Server::getPort() const { return _port; }

int Server::getSocket() const { return _listen_socket; }

int Server::getMaxBodySize() const { return _max_body_size; }

std::string Server::getHost() const { return _host; }


std::vector<std::string>    Server::getIndex() const { return _index;}

std::vector<std::string> Server::getServerNames() const { return _server_names; }

std::vector<std::string>::iterator Server::getBeginServerNames() { return _server_names.begin(); }; 
std::vector<std::string>::iterator Server::getEndServerNames() { return _server_names.end(); };

Client* Server::getClient(int i) const { return _clients[i]; }

std::vector<Location*>  Server::getLocations(void)const {return _locations;}

void Server::sortLocations()
{
    for(size_t j = 1; j < _locations.size(); j++)
    {
        Location* key = _locations[j];
        int i = j-1;

        while(i >= 0 && _locations[i]->getPath().length() < key->getPath().length())
        {
            _locations[i+1] = _locations[i];
            i--;
        }
        _locations[i+1] = key;
    }
}

std::vector<std::string>    Server::getMethods(void) const {return _methods;}

std::vector<Client*>::iterator Server::getBeginClients() { return _clients.begin(); }

std::vector<Client*>::iterator Server::getEndClients() { return _clients.end(); }

std::map<std::vector<unsigned int>, std::string> Server::getErrorPages() const { return _error_pages;}

std::string    Server::getCgiPath() const { return _cgi_path;};

bool        Server::getAutoIndex() const { return _auto_index; }

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
    if (error_page[0] == '/')
        error_page = error_page.substr(1);
    _error_pages.insert(std::make_pair(pages_numbers, error_page));
}
bool Server::setup(std::vector<Server*> servers)
{
    for (server_iterator it = servers.begin(); it != servers.end(); ++it)
    {
        if (*it != this && (*it)->getPort() == _port && (*it)->getHost() == _host )
            return false;
    }
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
    
    if (bind(_listen_socket, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) != 0)
        return false;
    return true;
}

bool Server::listenSocket()
{
    if (listen(_listen_socket, 10) != 0)
        return false;
    _is_listening = true;
    printf("Accepting connections on port %d.\n", _port);

    return true;
}

bool Server::isListening() const { return _is_listening; }

void Server::handleNewConnection()
{
    Client *new_client = new Client();
    new_client->setup(this);
    _clients.push_back(new_client);
}

void    Server::addLocation(Location *loc)
{
    _locations.push_back(loc);
}

void    Server::addIndex(std::vector<std::string> &index)
{
    if(!_index.empty())
        _index.clear();
    _index.assign(index.begin() + 1, index.end());
}

void    Server::addServerNames(std::vector<std::string> &server_names)
{
    if(!_server_names.empty())
        _server_names.clear();
    _server_names.assign(server_names.begin() + 1, server_names.end());
}

void    Server::addMethods(std::vector<std::string> &methods)
{
    if(!_methods.empty())
        _methods.clear();
    _methods.assign(methods.begin() + 1, methods.end());
}

Server * Server::clone() const
{
    return new Server(*this);
}

void Server::print(void)const
{
    std::cout << "host : " << _host << ":" << _port << std::endl;
    std::cout << "server_name : ";
    for (std::vector<std::string>::const_iterator it = _server_names.begin(); it != _server_names.end(); ++it)
        std::cout << *it << " ";
    std::cout  << std::endl;
    std::cout << "root : " << _root << std::endl;
    std::cout << "Client Max Body Size : " << _max_body_size << std::endl;
    for(std::vector<Location*>::const_iterator it = _locations.begin(); it != _locations.end(); ++it)
        (*it)->print();
    std::cout << std::endl;
}

const char	*Server::FailedSetup::what() const throw(){
	return "Failed to setup server";
}

