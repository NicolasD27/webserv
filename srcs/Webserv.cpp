/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:32:25 by clorin            #+#    #+#             */
/*   Updated: 2022/03/17 15:52:35 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(void):_conf_path(PATH_CONF){
    std::vector<Server*> _servers;
}

Webserv::~Webserv(void)
{
    std::cout << "deleting servers..." << std::endl;
    if(!_servers.empty())
        {
            for(iterator it = _servers.begin(); it != _servers.end(); ++it)
                delete (*it);
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
            for(iterator it = _servers.begin(); it != _servers.end(); ++it)
                delete (*it);
            _servers.clear();
        }
        
        throw BadConfiguration();
    }
    buff.close();
    if(!_servers.empty())
    {
        for (const_iterator it = _servers.begin(); it != _servers.end(); ++it)
        {   
            (*it)->sortLocations();
            
            if (!checkServer(*(*it)))
                throw BadConfiguration();           
        }
    }
    else
    {
        std::cerr << "file empty." << std::endl;
        throw BadConfiguration();
    }
}

bool Webserv::readConf(std::ifstream & buff)
{
    std::string str;
	
    buff.open(_conf_path);
    if (buff.fail())
    {
        std::cout << "File does not exist" << std::endl;
        return false;
    }
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
            std::cerr << "bad expected bracket" << std::endl << "\tUsage : server {"<< std::endl;
            return false;
        }
        if(!ParserConfig::check_block(buff, _servers))
            return false;
    }
    return true;
}

bool Webserv::checkHost(std::string const &host) const
{
    char *pch;
    char *test = strdup(host.c_str());
    pch = strtok(test, ".");
    unsigned int nb = 0;
    bool    ok = true;
    while(pch != NULL && ok)
    {
        size_t i = 0;
        bool is_digit = true;
        while(pch[i])
        {
            if (! isdigit(pch[i]))
            {
                is_digit = false;
                break;
            }
            i++;
        }
        if(is_digit)
        {
            int value = atoi(pch);
            if (value >=0 && value <= 255)
                nb++;
            else
                ok = false;
        }
        else
        {
            ok = false;
            break;
        }
        pch = strtok(NULL, ".");
    }
    free(test);
    free(pch);
    return (ok && nb == 4);
}

bool Webserv::checkLocations(std::vector<Location> &locations)
{
    for(size_t i = 0 ; i < locations.size(); i++)
    {
        std::cout << "Control of Location #"<< i << " ...";
        if(!locations[i].isValid())
            return false;
    }
    return true;
}

bool Webserv::checkServer(Server const & server) const
{
    if (server.getPort() == -1 || server.getServerNames().size() == 0)
        return false;
    if (server.getPort() <=0 || server.getPort() > 65535)
        return false;
    return (this->checkHost(server.getHost()));
}

void        Webserv::stop()
{
    _running = false;
}
bool        Webserv::run()
{
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;
    int highest_socket;

    for (iterator it = _servers.begin(); it != _servers.end(); ++it)
        if (!(*it)->setup(_servers))
            throw Server::FailedSetup();
    
    _running = true;
    while (_running)
    {
        highest_socket = 0;
        initFDSets(&read_fds, &write_fds, &except_fds);
        for (iterator it = _servers.begin(); it != _servers.end(); ++it)
            buildFDSets(**it, &read_fds, &write_fds, &except_fds, &highest_socket);        
        int select_res = select(highest_socket + 1, &read_fds, &write_fds, NULL, NULL);
        if (select_res <= 0)
            return false;
        else
        {
            for (iterator server_it = _servers.begin(); server_it != _servers.end(); ++server_it)
            {
                if (FD_ISSET((*server_it)->getSocket(), &read_fds))
                {
                    if (!(*server_it)->isListening())
                        (*server_it)->listenSocket();
                    else
                        (*server_it)->handleNewConnection();
                    break;
                }
                std::vector<int> client_to_remove;
                for (std::vector<Client*>::const_iterator client_it = (*server_it)->getBeginClients(); client_it != (*server_it)->getEndClients(); ++client_it)
                {
                    bool read_write = false;
                    if ((*client_it)->getResponseToBuildSize() > 0)
                    {
                        Response *to_switch = NULL;
                        for (std::vector<Response*>::iterator ite = (*client_it)->getBeginResponseToBuild(); ite != (*client_it)->getEndResponseToBuild(); ++ite)
                        {
                            if (!read_write && FD_ISSET((*ite)->getRessourceFD(), &read_fds))
                            {
                                if ((*ite)->getCGIReady())
                                    (*ite)->readCGI();
                                else
                                    (*ite)->readRessource();
                                to_switch = *ite;
                                read_write = true;
                                
                            }
                            if (!read_write && FD_ISSET((*ite)->getRessourceFD(), &write_fds))
                            {
                                read_write = true;
                                if(!(*ite)->executeCgi())
                                    to_switch = (*ite);
                            }
                        }
                        if (to_switch)
                            (*client_it)->switchToSendQueue(to_switch);
                    }
                    if (!read_write && (*client_it)->getSocket() != NO_SOCKET && FD_ISSET((*client_it)->getSocket(), &write_fds))
                    {
                        read_write = true;
                        if (!(*client_it)->sendToClient())
                            client_to_remove.push_back((*client_it)->getSocket());
                    }
                    if (!read_write && (*client_it)->getSocket() != NO_SOCKET && FD_ISSET((*client_it)->getSocket(), &read_fds))
                    {
                        read_write = true;
                        if (!(*client_it)->receiveFromClient(_servers))
                            client_to_remove.push_back((*client_it)->getSocket());
                    }
                }
                for (std::vector<int>::iterator ite = client_to_remove.begin(); ite != client_to_remove.end(); ++ite)
                    (*server_it)->removeClient(*ite);                
            }
        }
    }
    return false;
}




void Webserv::initFDSets(fd_set *read_fds, fd_set *write_fds, fd_set *except_fds)
{
    FD_ZERO(read_fds);
    FD_ZERO(write_fds);
    FD_ZERO(except_fds);
}

void Webserv::buildFDSets(Server const & server, fd_set *read_fds, fd_set *write_fds, fd_set *except_fds, int *highest_socket)
{
    if (server.getSocket() > *highest_socket)
        *highest_socket = server.getSocket();
    FD_SET(server.getSocket(), read_fds);
    for (std::vector<Client*>::const_iterator it = server.getBeginClients(); it != server.getEndClients(); ++it)
    {
        
        for (std::vector<Response*>::iterator ite = (*it)->getBeginResponseToBuild(); ite != (*it)->getEndResponseToBuild(); ++ite)
        {
            if ((*ite)->getRessourceFD() > *highest_socket)
                *highest_socket = (*ite)->getRessourceFD();
            if ((*ite)->getCGIReady() || (*ite)->getRessourceFD() > 0)
                FD_SET((*ite)->getRessourceFD(), read_fds);
            else
                FD_SET((*ite)->getRessourceFD(), write_fds);
        }
        if ((*it)->getSocket() != NO_SOCKET)
        {
            if ((*it)->getSocket() > *highest_socket)
                *highest_socket = (*it)->getSocket();
            FD_SET((*it)->getSocket(), read_fds);
            if ((*it)->hasResponseToSend())
                FD_SET((*it)->getSocket(), write_fds);
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
    }

}
/*exceptions*/
const char	*Webserv::BadConfiguration::what() const throw(){
	return "Bad configuration";
}


    
    