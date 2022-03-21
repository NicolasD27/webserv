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
#include <algorithm>
#include "file_utils.hpp"

class Response;


#include "utility.hpp"
#include "file_utils.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "StatusCode.hpp"
#include "MimesType.hpp"
#include "LanguageCodes.hpp"
#include "ErrorPages.hpp"
#include "CGIHandler.hpp"

class Response
{

private:
    Server    const                     *_pt_server;
    Request const                       *_pt_request;
    Location                            *_location_block;
    CGIHandler                          *_cgiHandler;
    std::string                         _body;
    bool                                _to_send;
    bool                                _cgiReady;
    long                                _ressource_fd;
    std::string                         _ressource_path;
    unsigned int                        _status;
    std::map<std::string, std::string>  _headers;
    // std::string                         _status_string;
    std::string                         _response_string;
    FILE                                *_CGIfOut;
    std::string                         _cgi_path;

    bool                                                buildRessourcePath(std::string, Location const &);
    void                                                buildErrorResponse(Server const & server);
    void                                                buildGetResponse(Request const & request, Server const & server);
    void                                                buildPostResponse(Request const & request, Server const & server);
    void                                                buildDeleteResponse(Request const & request, Server const & server);
    void                                                buildFileFD();
    bool                                                handleDots(std::string path);
    bool                                                findIndex(std::string current_directory, Location const &location);
    std::vector<std::string>                            findAlternativeMatches(std::string current_path);
    bool                                                chooseAcceptableFile(std::vector<std::string>);
    std::vector<std::map<std::string, std::string> >     parseAcceptableMIMETypes();
    std::vector<std::map<std::string, std::string> >     parseAcceptableLanguages();
    std::vector<std::map<std::string, std::string> >     buildTypeMap(std::vector<std::string> options);
    void                                                addDate();
    void                                                addLastModifiedDate();
    
public:
    Response(Request const & request, Server const & server);
    Response(unsigned int status, Server const & server);
    Response(Response const &);
    virtual ~Response();
    Response &operator=(Response const &);

    std::string                         buildResponseString();
    unsigned int                        readRessource();
    void                                readCGI();
    bool                                executeCgi();
    void                                CGIReady(long fd, FILE *CGIfOut);
    unsigned int                        buildAutoIndex();
    void                                parseExtension();
    void                                findLocation(Server const &, Request const &);

    std::string                         getResponseString() const;
    std::string                         getBody() const;
    bool                                isToSend() const;
    bool                                getCGIReady() const;
    std::string                         getRessourcePath(void) const;
    int                                 getRessourceFD() const;
    Request const *                     getRequest() const;
    int                                 getPort() const;
    std::string                         getHost() const;
    std::string                         getCgiPath() const;
    unsigned int                        getStatus(void) const;
    std::map<std::string, std::string>  getHeaders() const;
    Location *                          getLocationBlock() const;

    void                                printHeaders(std::ostream &);

};

std::ostream &operator <<(std::ostream &, Response &);
bool    compareByNameA(t_file &a, t_file &b);
bool    compareByNameD(t_file &a, t_file &b);
bool    compareBySizeA(t_file &a, t_file &b);
bool    compareBySizeD(t_file &a, t_file &b);
bool    compareByModifA(t_file &a, t_file &b);
bool    compareByModifD(t_file &a, t_file &b);
#endif
