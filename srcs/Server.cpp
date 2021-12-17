/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:33:58 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:37:53 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
}

Server::Server(Server const &cpy)
{
    if (this != &cpy) {
		*this = cpy;
	}
}

Server::~Server(){}

Server      &Server::operator=(Server const &cpy)
{
    if(this != &cpy)
    {
        //
    }
    return *this;
}
