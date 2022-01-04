/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:27:16 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:48:28 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

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

class Request;

#include "Server.hpp"


#define HTTP_UNKNOWN    0
#define HTTP_GET        1
#define HTTP_POST       2
#define HTTP_DELETE     3



class Request
{

private:
    std::string                         _request_string;
    unsigned int                        _http_method;
    std::string                         _location;
    // long                                _content_length;
    time_t                              _keep_alive_n;
    std::map<std::string, std::string>  _headers;
    std::vector<std::string>            _x_forwarded_for;
    std::string                         _body;
    
    
public:
    Request(std::string request_string);
    Request(Request const &);
    virtual ~Request();
    Request &operator=(Request const &);

    void parseHeaders();
    void storeHeader(std::string key, std::string value);
    void getMethod(std::string line);
    void printHeaders();

    
    
};

#endif