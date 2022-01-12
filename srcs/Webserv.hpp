/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:46:42 by clorin            #+#    #+#             */
/*   Updated: 2022/01/10 10:40:27 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility> 
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Server.hpp"
#include "ParserConfig.hpp"

# define PATH_CONF  "conf/default.conf"

class Webserv
{
private:
    std::vector<Server*>   _servers;
    std::string             _conf_path;

    bool checkHost(std::string const &host) const;
    
public:

    typedef std::vector<Server*>::iterator             iterator;
    typedef std::vector<Server*>::const_iterator       const_iterator;

    Webserv(); 
    Webserv(Webserv const &);
    virtual ~Webserv();
    Webserv &operator=(Webserv const &);

    void    config(std::string defaultPath = PATH_CONF);
    bool    readConf(std::ifstream & buff);
    bool    parseConf(std::ifstream & buff);
    bool    checkServer(Server const & server) const ;
    bool    run();
    void    shutdownServers();
    void    printServers();
    void    buildFDSets(Server const & server, fd_set *read_fds, fd_set *write_fds, fd_set *except_fds, int *highest_socket);
    void    initFDSets(fd_set *read_fds, fd_set *write_fds, fd_set *except_fds);

    class	BadConfiguration: public std::exception{
			virtual const char	*what() const throw();
		};

};

#endif