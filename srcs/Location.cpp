/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:51:26 by clorin            #+#    #+#             */
/*   Updated: 2022/01/20 18:13:23 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(void) : _autoIndex(false) {}

Location::Location(Location const &cpy):_path(cpy._path), _index(cpy._index),_root(cpy._root),_autoIndex(cpy._autoIndex),_methods(cpy._methods),_cgi_path(cpy._cgi_path){}

Location::Location(std::string path, std::vector<std::string> index, std::string root, bool autoIndex, std::vector<std::string> methods, std::string cgi_path):
_path(path), _index(index), _root(root), _autoIndex(autoIndex), _methods(methods), _cgi_path(cgi_path)
{}

Location &Location::operator=(Location const &cpy)
{
    if(&cpy != this)
    {
        _path = cpy._path;
        _root = cpy._root;
        _autoIndex = cpy._autoIndex;
        _cgi_path = cpy._cgi_path;
        _methods = cpy._methods;
        _index = cpy._index;
        _extensions = cpy._extensions;
    }
    return *this;
};

Location::~Location()
{
    _methods.clear();
    _index.clear();
}

std::string                 Location::getPath() const {return(_path);}

std::vector<std::string>    Location::getIndex() const {return(_index);}

std::string                 Location::getRoot() const {return _root;};

std::string                 Location::getCgiPath() const {return _cgi_path;}

bool                        Location::isAutoindex(void) const {return(_autoIndex);}

    
void            Location::setPath(std::string const &path)
{
    this->_path = path;
}

void            Location::setAutoIndex(bool autoIndex)
{
    this->_autoIndex = autoIndex;
}

void            Location::storeLine(std::string const & key, std::string & value)
{
    if (key == "autoindex")
        _autoIndex = (value == "on");
    else if (key == "root")
        _root = value;
}

void            Location::addMethods(std::vector<std::string> tokens)
{
    if(!_methods.empty())
        _methods.clear();
    _methods.assign(tokens.begin() + 1, tokens.end());
}


void            Location::addIndex(std::vector<std::string> tokens)
{
    if(! _index.empty())
        _index.clear();
    _index.assign(tokens.begin() + 1, tokens.end());
}

void            Location::addCgi(std::vector<std::string> tokens)
{
    if (tokens.size() == 3)
    {
        _cgi_path = tokens[1];
        _extensions = split(tokens[2], "|");
    }
}

bool            Location::hasExtension(std::string uri) const
{
    for (std::vector<std::string>::const_iterator it = _extensions.begin(); it != _extensions.end(); ++it)
    {
        if ((*it) == uri.substr(uri.length() - (*it).length(), (*it).length()))
            return true;
    }
    return false;
}

void            Location::print() const
{
    std::cout << "Location ("<<(&(*this))<<"): " << _path << std::endl;
    std::cout << "\troot : "<< _root << std::endl << "\tindex : ";
    if(_index.empty())
    {
        std::cout << C_RED << "empty" << C_RESET;
    }
    else
    {
        for(size_t i = 0; i < _index.size(); i++)
            std::cout <<_index[i] << " ";
    }
    std::cout << std::endl << "\tautoIndex = ";
    std::cout << ((_autoIndex)? "true":"false") << std::endl;
    if(!_methods.empty())
    {
        std::cout << "\tMethods : ";
        for(size_t i = 0; i < _methods.size(); i++)
            std::cout << _methods[i++] << " ";
        std::cout << std::endl;
    }
    std::cout << "Cgi_path = ";
    std::cout << ((_cgi_path.empty())? "Empty" : _cgi_path) << " for ";
    for (std::vector<std::string>::const_iterator it = _extensions.begin(); it != _extensions.end(); ++it)
        std::cout << *it << "|";
    std::cout << std::endl;
}

bool            Location::isValid() const
{
    // to continue
    return (_index.empty() == false);
}

bool            Location::hasMethod(std::string method) const
{
    if (_methods.size() == 0)
        return true;
    for(std::vector<std::string>::const_iterator it = _methods.begin(); it != _methods.end(); ++it)
        if ((*it) == method)
            return true;
    return false;
}