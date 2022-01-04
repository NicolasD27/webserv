/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:27:16 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:48:28 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <map>
#include <vector>
#include <utility>
#include <ctime>

#include "utility.hpp"
#include "Request.hpp"
#include "Server.hpp"

class Response
{

private:
    std::map<std::string, std::string>  _headers;
    unsigned int                        _status;
    // std::string                         _status_string;
    std::string                         _response_string;
    std::string                         _ressource_path;
    std::string                         _body;
    
    
public:
    Response(Request const & request, Server const & server);
    Response(Response const &);
    virtual ~Response();
    Response &operator=(Response const &);

    void addDate();
    std::string & getResponseString();
    void readRessource(std::stringstream & buff);
    void buildRessourcePath(Request const & request, Server const & server);

    

    
    
};

#endif
