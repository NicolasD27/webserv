/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:27:16 by clorin            #+#    #+#             */
/*   Updated: 2022/02/21 10:58:47 by clorin           ###   ########.fr       */
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
    std::string                                         _host;
    int                                                 _port;
    std::vector<std::string>                            _server_names;
    std::string                                         _root;
    int                                                 _listen_socket;
    std::vector<Client*>                                _clients;
    int                                                 _max_body_size;
    bool                                                _auto_index;
    std::map<std::vector<unsigned int>, std::string>    _error_pages;
    std::vector<Location*>                              _locations;
    std::vector<std::string>                            _methods;
    std::vector<std::string>                            _index;
    std::string                                         _cgi_path;
    bool                                                _is_listening;
public:
    typedef std::vector<Client*>::iterator              client_iterator;
    typedef std::vector<Client*>::const_iterator        client_const_iterator;
    typedef std::vector<Server*>::iterator              server_iterator;
    typedef std::vector<Server*>::const_iterator        server_const_iterator;
    typedef std::vector<Location*>::iterator            location_iterator;
    typedef std::vector<Location*>::const_iterator      location_const_iterator;

    Server(void);
    Server(Server const &);
    virtual ~Server();
    Server &operator=(Server const &);

    int                                                 getPort() const;
    int                                                 getSocket() const;
    std::string                                         getHost() const;
    std::vector<std::string>                            getIndex() const;
    std::vector<std::string>                            getServerNames() const;
    std::string                                         getCgiPath() const;
    int                                                 getMaxBodySize() const;
    Client*                                             getClient(int index) const;
    std::vector<Location*>                              getLocations(void) const;
    std::vector<std::string>                            getMethods(void) const;
    client_iterator                                     getBeginClients();
    client_iterator                                     getEndClients();
    bool                                                getAutoIndex() const;
    std::vector<std::string>::iterator                  getBeginServerNames(); 
    std::vector<std::string>::iterator                  getEndServerNames();
    std::map<std::vector<unsigned int>, std::string>    getErrorPages() const;
    int                                                 getNbClients() const;

    bool                                                setup(std::vector<Server*>);
    bool                                                listenSocket();
    bool                                                isListening() const;
    
    void                                                removeClient(int);
    void                                                storeLine(std::string & key, std::string & value);
    void                                                handleNewConnection(void);
    void                                                parseErrorPages(std::string & value);
    void                                                parseListen(std::string &);
    void                                                addLocation(Location *);
    void                                                sortLocations(void);
    void                                                addMethods(std::vector<std::string> &methods);
    void                                                addIndex(std::vector<std::string> &);
    void                                                addServerNames(std::vector<std::string> &);
    Server *                                            clone() const;

    void                                                print(void) const;

    class	FailedSetup: public std::exception{
			virtual const char	*what() const throw();
		};
    
};

#endif