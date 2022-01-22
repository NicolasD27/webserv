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
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
class Response;


#include "utility.hpp"
#include "file_utils.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "StatusCode.hpp"
#include "MimesType.hpp"
#include "ErrorPages.hpp"
#include "CGIHandler.hpp"

class Response
{

private:
    std::map<std::string, std::string>  _headers;
    unsigned int                        _status;
    // std::string                         _status_string;
    std::string                         _response_string;
    std::string                         _ressource_path;
    int                                 _ressource_fd;
    std::string                         _body;
    Request const                       *_pt_request;
    Server    const                     *_pt_server;
    bool                                _to_send;
    Location                            _location_block;

    void findLocation(std::string const &, Server const &, Request const &);
    bool buildRessourcePath(std::string const &, Location const &);
    void buildErrorResponse(Server const & server);
    void buildGetResponse(Request const & request, Server const & server);
    void buildPostResponse(Request const & request, Server const & server);
    void buildDeleteResponse(Request const & request, Server const & server);
    
    std::string		executeCgi(const char **scriptName, const std::string & body, char **env);
    std::string  _cgi_path;
    
public:
    Response(Request const & request, Server const & server);
    Response(Response const &);
    virtual ~Response();
    Response &operator=(Response const &);

    void addDate();
    std::string buildResponseString();
    std::string getResponseString() const;
    bool isToSend() const;
    unsigned int readRessource(bool isErrorPage = false);

    unsigned int buildAutoIndex();
    void parseExtension();
    std::string getRessourcePath(void) const;
    int getRessourceFD() const;
    Request const * getRequest() const;

    unsigned int    getStatus(void) const;

    void printHeaders(std::ostream &);

};

std::ostream &operator <<(std::ostream &, Response &);

#endif
