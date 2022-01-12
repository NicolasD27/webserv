/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 10:42:37 by clorin            #+#    #+#             */
/*   Updated: 2022/01/12 10:12:16 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserConfig.hpp"

std::vector<std::string> ParserConfig::form_inst_line(std::ifstream &buff)
{
    std::string line;

    std::getline(buff, line);
    line = removeComments(line);
    line = trim(line);
    return(split(line));
}

std::string     ParserConfig::removeComments(std::string const &str)
{
    std::string     ret = str;
    size_t          posCharComments;

    posCharComments = str.find('#');
    if(posCharComments == std::string::npos)
        return ret;
    return(ret.substr(0, posCharComments));
}

std::string     ParserConfig::trim(std::string const &str)
{
    std::string     ret = str;
    size_t          start, end;

    start = ret.find_first_not_of(WHITESPACES);
    if(start != std::string::npos)
        ret = ret.substr(start);
    end = ret.find_last_not_of(WHITESPACES);
    if(end != std::string::npos)
        ret = ret.substr(0, end + 1);
    return ret;
}

std::vector<std::string>    ParserConfig::split(std::string const &str)
{
    std::vector<std::string>    ret;
    std::string                 token;
    size_t                      i = 0, pos, len;

    while(i < str.size())
    {
        pos = str.find_first_of(WHITESPACES, i);
        if(pos == std::string::npos)
            break;
        len = pos - i;
        token = str.substr(i, len);
        i += len + 1;
        if(token.empty() == false)
            ret.push_back(token);
    }
    if(i < str.size())
        ret.push_back(str.substr(i));
    return (ret);
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
            return false;
        }
        if(tokens[0] == "location")
        {
            //std::cout << "Check Location block\n";
            if (!check_location_block(buff, tokens, new_server))
                return false;
            //std::cout << "------------\n";
        }
        else
        {
            if(removeSemicolon(tokens))
                new_server->storeLine(tokens[0], tokens[1]);
            else
            {
                std::cerr << "Expected ';' at the end of line." << std::endl;
                delete new_server;
                return false;
            }
            
        }
    }
    if (!closed)
    {
        std::cerr << "bracket } not found." << std::endl;
        delete new_server;
        return false;
    }    
    servers.push_back(new_server);
    //std::cout << "********\n";
    return true;
}

bool ParserConfig::check_location_block(std::ifstream &buff, std::vector<std::string> const &tokens, Server *server)
{
    bool                        closed = false;
    std::string                 path;
    Location                    newLocation;
    std::vector<std::string>    locationTokens;

    if(tokens.size() != 3)
    {
        std::cerr << "Bad format for location block: Usage: 'location [/path/name] {'" << std::endl;
        return false;
    }
    path = tokens[1];
    if(tokens[2] != "{")
    {
        std::cerr << "Expected '{' in location block." << std::endl;
        return false;
    }
    newLocation.setPath(path);
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
        if(removeSemicolon(locationTokens))
        { 
            if(locationTokens[0] == "methods")
            {
                newLocation.addMethods(locationTokens);
            }
            else
                newLocation.storeLine(locationTokens[0], locationTokens[1]);
        }
        else
        {
            std::cerr << "Expected ';' at the end of line." << std::endl;
            return false;
        }
    }

    if (!closed)
    {
        std::cerr << "bracket } not found." << std::endl;
        return false;
    }    
    server->addLocation(newLocation);
    return true;
}