/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:27:16 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:48:28 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Client.hpp"

class Server;
#include "Webserv.hpp"

#define MAX_CLIENTS 10

#define SERVER_IPV4_ADDR "127.0.0.1" // à load depuis la config ?

class Server
{
private:
    int                     _port;
    std::string             _host;
    std::string             _server_name;  
    int                     _listen_socket;
    std::vector<Client*>    _clients;
    int                     _max_body_size;
    
public:
    typedef typename std::vector<Client*>::iterator             iterator;
    typedef typename std::vector<Client*>::const_iterator       const_iterator;

    Server(void);
    Server(int port, std::string server_name, int max_body_size);
    Server(Server const &);
    virtual ~Server();
    Server &operator=(Server const &);

    int getPort() const;
    int getSocket() const;
    std::string getHost() const;
    std::string getServerName() const;
    Client* getClient(int index) const;
    const_iterator getBeginClients() const;
    const_iterator getEndClients() const;

    bool setup();
    
    void storeLine(std::string & key, std::string & value);
    void handleNewConnection(void);


    class	FailedSetup: public std::exception{
			virtual const char	*what() const throw();
		};
    
};

#endif