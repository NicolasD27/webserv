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
#include <stack>
#include <utility>
#include <ctime>
#include <dirent.h>

#include "utility.hpp"
#include "file_utils.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "StatusCode.hpp"
#include "MimesType.hpp"
#include "ErrorPages.hpp"

class Response
{

private:
    std::map<std::string, std::string>  _headers;
    unsigned int                        _status;
    // std::string                         _status_string;
    std::string                         _response_string;
    std::string                         _ressource_path;
    std::string                         _body;
    Request const                       *_pt_request;
    Server    const                     *_pt_server;

    Location                            findLocation(std::string const &, Server const &);
    void buildRessourcePath(std::string const &, Location const &);
    
public:
    Response(Request const & request, Server const & server);
    Response(Response const &);
    virtual ~Response();
    Response &operator=(Response const &);

    void addDate();
    std::string & getResponseString();
    unsigned int readRessource(bool isErrorPage = false);

    unsigned int buildAutoIndex();
    void parseExtension();
    std::string & getRessourcePath(void);

    unsigned int    getStatus(void) const;

    void printHeaders(std::ostream &);

};

std::ostream &operator <<(std::ostream &, Response &);

#endif
