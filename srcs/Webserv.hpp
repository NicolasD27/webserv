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
#include <map>
#include "Server.hpp"

# define PATH_CONF  "./srcs/default.conf"

class Webserv
{
private:
    std::map<int, Server>   _server;
    std::string             _conf_path;
    
public:
    Webserv(); 
    Webserv(Webserv const &);
    virtual ~Webserv();
    Webserv &operator=(Webserv const &);

    void    config(std::string defaultPath = PATH_CONF);
    bool    run();

    class	BadConfiguration: public std::exception{
			virtual const char	*what() const throw();
		};

};

#endif