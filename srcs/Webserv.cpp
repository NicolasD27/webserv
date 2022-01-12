/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:32:25 by clorin            #+#    #+#             */
/*   Updated: 2022/01/12 11:02:26 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(void):_conf_path(PATH_CONF){
    std::vector<Server*> _servers;
}

Webserv::~Webserv(void)
{
    if(!_servers.empty())
        {
            for(size_t i = 0; i < _servers.size(); i++)
                delete _servers[i];
            _servers.clear();
        }
}

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
    std::ifstream buff;
    _conf_path = conf_path;
    if (!readConf(buff))
        throw BadConfiguration();
    if (!parseConf(buff))
    {   
        buff.close();
        if(!_servers.empty())
        {
            for(size_t i = 0; i < _servers.size(); i++)
                delete _servers[i];
            _servers.clear();
        }

        throw BadConfiguration();
    }
    buff.close();
}

bool Webserv::readConf(std::ifstream & buff)
{
    std::string str;
	
    //std::ifstream ifs(_conf_path);
    buff.open(_conf_path);
    if (buff.fail())
    {
        std::cout << "File does not exist" << std::endl;
        return false;
    }
	
	// while (std::getline(ifs, str))
    //     buff << str << std::endl;
	//ifs.close();
    return true;
}



bool    Webserv::parseConf(std::ifstream & buff)
{
    std::vector<std::string> tokens;

    while(buff)
    {
        tokens = ParserConfig::form_inst_line(buff);
        
        if(tokens.empty())
            continue;
        if(tokens[0] != "server")
        {
            std::cerr << "unknow instruction : " << tokens[0] << std::endl;
            return false;
        }
        if(tokens.size() == 1 || (tokens.size() == 2 && tokens[1] != "{"))
        {
            std::cerr << "bad expected bracket" << std::endl;
            return false;
        }
        if(!ParserConfig::check_block(buff, _servers))
            return false;
    }
    return true;
}
// bool Webserv::parseConf(std::stringstream & buff)
// {
    
//     Server *new_server;
//     std::string line;
//     int open = 0;
//     while(std::getline(buff, line))
//     {
//         if (line.length() == 0)
//             continue;
//         std::string::size_type begin = line.find_first_not_of(" ");
//         std::string::size_type end   = line.find_last_not_of(" ");
//         line = line.substr(begin, end-begin + 1);
//         std::string key;
//         std::istringstream iss_line(line);        
//         if (line == "server")
//             new_server = new Server();  
//         else if (line == "{")
//             open++;
//         else if (line == "}")
//         {
//             if (open <= 0)
//                 return false;
//             open--;
//             if (!checkServer(*new_server))
//                 return false;
//             _servers.push_back(new_server);            
//         }
//         else if( std::getline(iss_line, key, '=') )
//         {
//             key = key.substr(1, key.length() - 1); // je considère qu'il y a un seul \t au début
//             std::string value;
//             if( std::getline(iss_line, value) )
//                 new_server->storeLine(key, value);
//         }

//     //     //ajouter parser pour les location { ...}
//     }
//     if (open < 0)
//         return false;
//     return true;
// }

bool Webserv::checkServer(Server const & server) const
{
    if (server.getPort() == -1 || server.getServerName() == "")
        return false;
    return true;
}

bool        Webserv::run()
{
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;
    int highest_socket;

    for (iterator it = _servers.begin(); it != _servers.end(); ++it)
        if (!(*it)->setup())
            throw Server::FailedSetup();
    

    while (1)
    {
        highest_socket = 0;
        initFDSets(&read_fds, &write_fds, &except_fds);
        for (iterator it = _servers.begin(); it != _servers.end(); ++it)
            buildFDSets(**it, &read_fds, &write_fds, &except_fds, &highest_socket);
        std::cout << "before select" << std::endl;
        int select_res = select(highest_socket + 1, &read_fds, &write_fds, &except_fds, NULL);
        std::cout << "select_res : " << select_res << std::endl;
        if (select_res <= 0)
            return false;
        else
        {
            for (iterator server_it = _servers.begin(); server_it != _servers.end(); ++server_it)
            {
                if (FD_ISSET((*server_it)->getSocket(), &read_fds)) {
                    std::cout << "new connection "  << std::endl;
                    (*server_it)->handleNewConnection();
                }
            
                for (std::vector<Client*>::const_iterator client_it = (*server_it)->getBeginClients(); client_it != (*server_it)->getEndClients(); ++client_it)
                {
                    if ((*client_it)->getSocket() != NO_SOCKET && FD_ISSET((*client_it)->getSocket(), &read_fds))
                    {

                        (*client_it)->receiveFromClient();

                        
                    }
                    if ((*client_it)->getSocket() != NO_SOCKET && FD_ISSET((*client_it)->getSocket(), &write_fds))
                    {
                        (*client_it)->sendToClient();
                        
                    }

                }
                
            }
            std::cout << "end" << std::endl;
        }
    }
    return true;
}




void Webserv::initFDSets(fd_set *read_fds, fd_set *write_fds, fd_set *except_fds)
{
    FD_ZERO(read_fds);
    FD_ZERO(write_fds);
    FD_ZERO(except_fds);
    // FD_SET(0, read_fds);
}

void Webserv::buildFDSets(Server const & server, fd_set *read_fds, fd_set *write_fds, fd_set *except_fds, int *highest_socket)
{
    if (server.getSocket() > *highest_socket)
        *highest_socket = server.getSocket();
    FD_SET(server.getSocket(), read_fds);
    FD_SET(server.getSocket(), except_fds);
    for (std::vector<Client*>::const_iterator it = server.getBeginClients(); it != server.getEndClients(); ++it)
    {
        if ((*it)->getSocket() != NO_SOCKET)
        {
            if ((*it)->getSocket() > *highest_socket)
                *highest_socket = (*it)->getSocket();
            FD_SET((*it)->getSocket(), read_fds);
            FD_SET((*it)->getSocket(), except_fds);
            if ((*it)->hasMessages() && (*it)->getCurrentMessage().length() > 0)
            {
                // std::cout << "message is : " << (*it)->getCurrentMessage() << std::endl;
                FD_SET((*it)->getSocket(), write_fds);
            }
        }
    }
}

void Webserv::shutdownServers()
{
    for (iterator it = _servers.begin(); it != _servers.end(); ++it)
        delete *it;
    exit(EXIT_FAILURE);
}

void Webserv::printServers() 
{
    for (iterator it = _servers.begin(); it != _servers.end(); ++it)
    {
        std::cout << "servers : ( " <<&(*it)<< " )" << std::endl;
        (*it)->print();
        // std::cout << "host : " << (*it)->getHost() << ":" << (*it)->getPort() << std::endl;
        // std::cout << "server_name : " << (*it)->getServerName() << std::endl;
        // std::cout << "root : " << (*it)->getRoot() << std::endl << std::endl;
    }

}
/*exceptions*/
const char	*Webserv::BadConfiguration::what() const throw(){
	return "Bad configuration";
}


    
    