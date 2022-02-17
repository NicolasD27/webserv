/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 12:50:58 by clorin            #+#    #+#             */
/*   Updated: 2022/01/10 13:48:29 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPages.hpp"

ErrorPages::ErrorPages()
{
    _errorPage[400] = PAGE400;
    _errorPage[403] = PAGE403;
    _errorPage[404] = PAGE404;
    _errorPage[405] = PAGE405;
    _errorPage[414] = PAGE414;
    _errorPage[500] = PAGE500;
}


ErrorPages::~ErrorPages()
{
    _errorPage.clear();
}

std::string     &ErrorPages::operator[](unsigned int status)
{
    return (_errorPage[status]);
}