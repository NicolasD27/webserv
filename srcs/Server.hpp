/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:27:16 by clorin            #+#    #+#             */
/*   Updated: 2022/01/11 15:26:54 by clorin           ###   ########.fr       */
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
    int                     _port;
    std::string             _host;
    std::string             _server_name;
    std::string             _root;
    std::string             _index;
    int                     _listen_socket;
    std::vector<Client*>    _clients;
    int                     _max_body_size;
    bool                    _auto_index;
    std::map<std::vector<unsigned int>, std::string>    _error_pages;
    std::vector<Location>   _locations;
    
public:
    typedef std::vector<Client*>::iterator             iterator;
    typedef std::vector<Client*>::const_iterator       const_iterator;

    Server(void);
    Server(int port, std::string server_name, int max_body_size);
    Server(Server const &);
    virtual ~Server();
    Server &operator=(Server const &);

    int getPort() const;
    int getSocket() const;
    std::string getHost() const;
    std::string getRoot() const;
    std::string getIndex() const;
    std::string getServerName() const;
    Client* getClient(int index) const;
    const_iterator getBeginClients() const;
    const_iterator getEndClients() const;
    bool           getAutoIndex() const;

    bool setup();
    
    void storeLine(std::string & key, std::string & value);
    void handleNewConnection(void);
    void parseErrorPages(std::string & value);
    std::map<std::vector<unsigned int>, std::string> getErrorPages() const;

    void    addLocation(Location &);

    void    print(void) const;

    class	FailedSetup: public std::exception{
			virtual const char	*what() const throw();
		};
    
};

#endif