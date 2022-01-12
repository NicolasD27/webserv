/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:51:26 by clorin            #+#    #+#             */
/*   Updated: 2022/01/12 09:44:19 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(Location const &cpy):_path(cpy._path), _index(cpy._index),_root(cpy._root),_autoIndex(cpy._autoIndex),_methods(cpy._methods){}

Location::~Location()
{
    _methods.clear();
}

std::string     Location::getPath() const {return(_path);}

std::string     Location::getIndex() const {return(_index);}

std::string     Location::getRoot() const {return _root;};

bool            Location::isAutoindex(void) const {return(_autoIndex);}
    
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
    else if (key == "root")
        _root = value;
}

void            Location::addMethods(std::vector<std::string> &tokens)
{
    if(!_methods.empty())
        _methods.clear();
    _methods.assign(tokens.begin() + 1, tokens.end());
}

void            Location::print() const
{
    std::cout << "Location : " << _path << std::endl;
    std::cout << "\troot : "<< _root << std::endl << "\tindex : " <<_index << std::endl << "\tautoIndex = ";
    std::cout << ((_autoIndex)? "true":"false") << std::endl;
    if(!_methods.empty())
    {
        std::cout << "\tMethods : ";
        for(size_t i = 0; i < _methods.size(); i++)
            std::cout << _methods[i++] << " ";
        std::cout << std::endl;
    }
}

