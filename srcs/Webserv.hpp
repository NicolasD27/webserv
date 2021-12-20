/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:46:42 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:47:01 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility> 
#include <vector>
#include "Server.hpp"

# define PATH_CONF  "./srcs/default.conf"

class Webserv
{
private:
    std::vector<Server>   _servers;
    std::string             _conf_path;
    
public:
    Webserv(); 
    Webserv(Webserv const &);
    virtual ~Webserv();
    Webserv &operator=(Webserv const &);

    void    config(std::string defaultPath = PATH_CONF);
    bool    readConf(std::stringstream & buff);
    bool    parseConf(std::stringstream & buff);
    bool    checkServer(Server const & server) const ;
    bool    run();
    void    printServers() const;

    class	BadConfiguration: public std::exception{
			virtual const char	*what() const throw();
		};

};

#endif