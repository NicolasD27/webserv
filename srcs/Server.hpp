/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:27:16 by clorin            #+#    #+#             */
/*   Updated: 2022/01/20 17:08:56 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Server;
#include "Client.hpp"

#include "Webserv.hpp"
#include "Location.hpp"

#define MAX_CLIENTS 10

#define SERVER_IPV4_ADDR "127.0.0.1" // à load depuis la config ?

class Server
{
private:
    int                         _port;
    std::string                 _host;
    std::vector<std::string>    _server_names;
    std::string                 _root;
    int                         _listen_socket;
    std::vector<Client*>        _clients;
    int                         _max_body_size;
    bool                        _auto_index;
    std::map<std::vector<unsigned int>, std::string>    _error_pages;
    std::vector<Location>       _locations;
    std::vector<std::string>    _methods;
    std::vector<std::string>    _index;
    std::string                 _cgi_path;
    
public:
    typedef std::vector<Client*>::iterator             client_iterator;
    typedef std::vector<Client*>::const_iterator       client_const_iterator;
    typedef std::vector<Server*>::iterator             server_iterator;
    typedef std::vector<Server*>::const_iterator       server_const_iterator;

    Server(void);
    Server(Server const &);
    virtual ~Server();
    Server &operator=(Server const &);

    int getPort() const;
    int getSocket() const;
    std::string getHost() const;
    std::string getRoot() const;
    std::vector<std::string> getIndex() const;
    std::vector<std::string> getServerNames() const;
    std::string getCgiPath() const;

    Client* getClient(int index) const;
    std::vector<Location>       getLocation(void) const;
    std::vector<std::string>    getMethods(void) const;
    client_const_iterator getBeginClients() const;
    client_const_iterator getEndClients() const;
    bool           getAutoIndex() const;
    std::vector<std::string>::iterator getBeginServerNames(); 
    std::vector<std::string>::iterator getEndServerNames();
    void removeClient(client_const_iterator);

    bool setup(std::vector<Server*>);
    
    void storeLine(std::string & key, std::string & value);
    void setMethods(std::vector<std::string> &);
    void handleNewConnection(void);
    void parseErrorPages(std::string & value);
    std::map<std::vector<unsigned int>, std::string> getErrorPages() const;

    void    addLocation(Location &);
    void    sortLocations(void);
    void    addMethods(std::string &);
    void    addIndex(std::vector<std::string> &);
    void    addServerNames(std::vector<std::string> &);

    void    print(void) const;

    class	FailedSetup: public std::exception{
			virtual const char	*what() const throw();
		};
    
};

#endif