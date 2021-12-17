/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:46:42 by clorin            #+#    #+#             */
/*   Updated: 2021/12/15 15:00:40 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <iostream>
#include <map>

class server;

class webserv
{
private:
    std::map<int, server>   _server;
    
public:
    webserv(void);
    webserv(std::string &); //Si on a un fichier par default dans ce constructeur on peut supprimer le constructeurr(void)
    webserv(webserv const &);
    virtual ~webserv();
    webserv &operator=(webserv const &);

    bool    run();

};

#endif