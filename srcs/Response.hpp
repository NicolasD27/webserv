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
#include <dirent.h>

#include "utility.hpp"
#include "file_utils.hpp"
#include "Request.hpp"
#include "Server.hpp"

# define PAGE403    "<h1>Forbidden</h1><p>You don't have permission to access this resource.</p>"
# define PAGE404    "<h1 style='color:red'>Error 404: Not found :-(</h1><p>I have no idea where that file is, sorry.  Are you sure you typed in the correct URL?</p>"
# define PAGE500    "<h1>Oops! Something went wrong...</h1><p>We seem to be having some technical difficulties. Hang tight.</p>"

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
    
public:
    Response(Request const & request, Server const & server);
    Response(Response const &);
    virtual ~Response();
    Response &operator=(Response const &);

    void addDate();
    std::string & getResponseString();
    void readRessource(bool isErrorPage = false);
    void buildRessourcePath(Request const & request, Server const & server);
    void buildAutoIndex();
    void parseExtension();
    std::string & getRessourcePath(void);

    unsigned int    getStatus(void) const;

    void printHeaders(std::ostream &);

};

std::ostream &operator <<(std::ostream &, Response &);

#endif
