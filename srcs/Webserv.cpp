/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:32:25 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:49:04 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(void):_conf_path(PATH_CONF){
    
}

Webserv::~Webserv(void){}

Webserv::Webserv(Webserv const &cpy)
{
    if (this != &cpy) {
		*this = cpy;
	}
}

Webserv     &Webserv::operator=(Webserv const &cpy)
{
    if(this != &cpy)
    {
        this->_conf_path = cpy._conf_path;
        this->_servers = cpy._servers;
    }
    return *this;
}

void        Webserv::config(std::string conf_path)
{
    this->_conf_path = conf_path;
    /*chargement du fichier
        Parsing
        verification
        remplissage de _server
    */
    std::stringstream buff;
    if (!readConf(buff))
        throw BadConfiguration();
    if (!parseConf(buff))
        throw BadConfiguration();
}

bool Webserv::readConf(std::stringstream & buff)
{
    std::string str;
	
    std::ifstream ifs(_conf_path);

    if (ifs.fail())
    {
        std::cout << "File does not exist" << std::endl;
        return false;
    }
	
	while (std::getline(ifs, str))
        buff << str << std::endl;
	ifs.close();
    return true;
}

bool Webserv::parseConf(std::stringstream & buff)
{
    std::vector<Server> _servers;
    Server new_server;
    std::string line;
    int open = 0;
    while(std::getline(buff, line))
    {
        std::string::size_type begin = line.find_first_not_of(" ");
        std::string::size_type end   = line.find_last_not_of(" ");

        line = line.substr(begin, end-begin + 1);
        std::string key;
        std::istringstream is_line(line);
        if (line == "server")
            new_server = Server();  
        else if (line == "{")
            open++;
        else if (line == "}")
        {
            if (open <= 0)
                return false;
            open--;
            if (!checkServer(new_server))
                return false;
            _servers.push_back(new_server);
            std::cout << "host : " << new_server.getHost() << ":" << new_server.getPort() << std::endl;
            std::cout << "server_name : " << new_server.getServerName() << std::endl;
            std::cout << "end" << std::endl;
            std::cout << "server name : " << _servers.front().getServerName() << std::endl;
            printServers();
        }
        else if( std::getline(is_line, key, '=') )
        {
            key = key.substr(1, key.length() - 1); // je considère qu'il y a un seul \t au début
            std::string value;
            std::cout << "key : " << key << " length : " << key.length() << std::endl;
            if( std::getline(is_line, value) )
            {
                std::cout << "value : " << value << std::endl;
                new_server.storeLine(key, value);
            }
            std::cout << "after store" << std::endl;
            std::cout << "host : " << new_server.getHost() << ":" << new_server.getPort() << std::endl;
            std::cout << "server_name : " << new_server.getServerName() << std::endl;
        }

    //     //ajouter parser pour les location { ...}
    }
    if (open < 0)
        return false;
    return true;
}

bool Webserv::checkServer(Server const & server) const
{
    if (server.getPort() == -1 || server.getServerName() == "")
        return false;
    return true;
}

bool        Webserv::run()
{
    /* running*/
    return true;
}

void Webserv::printServers() const 
{
    std::cout << "servers : " << std::endl;
    for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        
        std::cout << "host : " << it->getHost() << ":" << it->getPort() << std::endl;
        std::cout << "server_name : " << it->getServerName() << std::endl << std::endl;
        
    }

}
/*exceptions*/
const char	*Webserv::BadConfiguration::what() const throw(){
	return "Bad configuration";
}

    
    