/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 10:42:37 by clorin            #+#    #+#             */
/*   Updated: 2022/02/23 08:30:44 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserConfig.hpp"

std::vector<std::string> ParserConfig::form_inst_line(std::ifstream &buff)
{
    std::string line;

    std::getline(buff, line);
    line = removeComments(line);
    line = trim(line);
    return(split(line, WHITESPACES));
}



bool                  ParserConfig::removeSemicolon(std::vector<std::string> &tokens)
{
    std::string             lastToken = tokens.back();
    std::string::iterator   it = tokens.back().end();
    char                    lastChar = *(--it);
       
    if(lastToken == ";")
    {
        tokens.pop_back();
        return true;
    }
    if(lastChar != ';')
        return false;
    tokens.back().erase(it);
    return true;
}

bool ParserConfig::check_block(std::ifstream &buff, std::vector<Server*> &servers)
{
    std::vector<std::string> tokens;
    bool    closed = false;
    Server *new_server = new Server();
    Location *default_loc = new Location();

    default_loc->setPath("/");
    //std::cout << "check block\n";
    while(buff)
    {
        tokens = ParserConfig::form_inst_line(buff);
        if(tokens.empty())
            continue;
        if(tokens[0] == "}")
        {
            closed = true;
            break;
        }
        if(tokens[0] == "server")
        {
            std::cerr << "bracket } not found." << std::endl;
            delete new_server;
            delete default_loc;
            return false;
        }
        if(tokens[0] == "location")
        {
            if (!check_location_block(buff, tokens, new_server, default_loc))
            {
                delete new_server;
                delete default_loc;
                return false;
            }
        }
        else
        {
            if(removeSemicolon(tokens))
            {
                if (tokens[0] == "server_name")
                    new_server->addServerNames(tokens);
                else if (tokens[0] == "return")
                    default_loc->addRedirection(tokens);
                else if(tokens[0] == "index")
                    default_loc->addIndex(tokens);
                else if (tokens[0] == "methods")
                    default_loc->addMethods(tokens);
                else if (tokens[0] == "cgi")
                    default_loc->addCgi(tokens);
                else if (tokens[0] == "root" || tokens[0] == "autoindex" || tokens[0] == "upload_dir" || tokens[0] == "max_body_size")
                    default_loc->storeLine(tokens[0], tokens[1]);
                else
                    new_server->storeLine(tokens[0], tokens[1]);
            }
            else
            {
                std::cerr << "Expected ';' at the end of line." << std::endl;
                delete new_server;
                delete default_loc;
                return false;
            }
            
        }
    }
    if (!closed)
    {
        std::cerr << "bracket } not found." << std::endl;
        delete new_server;
        delete default_loc;
        return false;
    }
    if (default_loc)
        new_server->addLocation(default_loc);
    servers.push_back(new_server);
    
    return true;
}

bool ParserConfig::check_location_block(std::ifstream &buff, std::vector<std::string> const &tokens, Server *server, Location * default_loc)
{
    bool                        closed = false;
    std::string                 path;
    Location                    *newLocation = new Location();
    std::vector<std::string>    locationTokens;

    if(tokens.size() != 3)
    {
        std::cerr << "Bad format for location block: Usage: 'location [/path/name] {'" << std::endl;
        delete newLocation;
        return false;
    }
    path = tokens[1];
    if (path == "/")
    {
        
        delete newLocation;
        newLocation = default_loc;
        // delete default_loc;
        // default_loc = NULL;
    }
    if(tokens[2] != "{")
    {
        std::cerr << "Expected '{' in location block." << std::endl;
        delete newLocation;
        return false;
    }
    newLocation->setPath(path);
    while(buff)
    {
        locationTokens = ParserConfig::form_inst_line(buff);
        if(locationTokens.size() == 0)
            continue;
        if(locationTokens[0] == "}")
        {
            closed = true;
            break;
        }
        if(locationTokens[0] == "location")
        {
            std::cerr << "bracket } not found." << std::endl;
            delete newLocation;
            return false;
        }
        if(removeSemicolon(locationTokens))
        { 
            if(locationTokens[0] == "methods")
                newLocation->addMethods(locationTokens);
            else if(locationTokens[0] == "index")
                newLocation->addIndex(locationTokens);
            else if (locationTokens[0] == "cgi")
                newLocation->addCgi(locationTokens);
            else if (locationTokens[0] == "return")
                newLocation->addRedirection(locationTokens);
            else
                newLocation->storeLine(locationTokens[0], locationTokens[1]);
            
        }
        else
        {
            std::cerr << "Expected ';' at the end of line." << std::endl;
            delete newLocation;
            return false;
        }
    }
    if (!closed)
    {
        std::cerr << "bracket } not found." << std::endl;
        delete newLocation;
        return false;
    }    
    if (path == "/")
        default_loc = newLocation;
    else
        server->addLocation(newLocation);
    return true;
}