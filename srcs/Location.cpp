/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:51:26 by clorin            #+#    #+#             */
/*   Updated: 2022/01/11 15:32:40 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(Location const &cpy):_path(cpy._path), _index(cpy._index),_autoIndex(cpy._autoIndex){}

std::string     Location::getPath(void)const
{
    return(this->_path);
}

std::string     Location::getIndex(void)const
{
    return(this->_index);
}

bool            Location::isAutoindex(void)const
{
    return(this->_autoIndex);
}
    
void            Location::setPath(std::string &path)
{
    this->_path = path;
}

void            Location::setAutoIndex(bool autoIndex)
{
    this->_autoIndex = autoIndex;
}

void            Location::storeLine(std::string & key, std::string & value)
{
    if (key == "index")
        _index = value;
    else if (key == "autoindex")
        _autoIndex = (value == "on");
}

void            Location::print() const
{
    std::cout << "Location : in " << _path << std::endl << "\t index : " <<_index << std::endl << "\t autoIndex = ";
    std::cout << ((_autoIndex)? "true":"false") << std::endl;
}
