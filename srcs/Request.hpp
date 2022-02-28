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
#include "Colors.hpp"

class Request;

#include "ParserConfig.hpp"

#define HTTP_UNKNOWN    0
#define HTTP_GET        1
#define HTTP_POST       2
#define HTTP_DELETE     3


class Request
{

private:
    std::string                         _http_version;
    std::string                         _request_string;
    std::string                         _query_string;
    std::string                         _http_method;
    std::string                         _location;
    time_t                              _keep_alive_n;
    std::map<std::string, std::string>  _headers;
    std::string                         _body;
    std::map<std::string, std::string>  _params;
    uint16_t                            _portClient;
    std::string                         _addressClient;
    bool                                _format_error;
    void                                handleLocation(std::string);
    
public:
    Request(std::string request_string);
    Request(Request const &);
    virtual ~Request();

    Request &                           operator=(Request const &);
    std::string &                       operator[](const char *key);

    void                                parseHeaders(std::string request_string);
    void                                storeHeader(std::string key, std::string value);
    void                                parseMethod(std::string line);
    void                                printHeaders(std::ostream &);
    void                                printParams(std::ostream &);
    void                                addToBody(std::string new_elem);
    void                                setPortClient(uint16_t);
    void                                setAddressClient(std::string &);

    std::string                         getHttpMethod(void) const;
    std::string                         getLocation(void) const;
    time_t                              getKeepAliveN() const;
    std::string                         getBody() const;
    std::map<std::string, std::string>  getParams() const;
    std::string                         getRequestString() const;
    std::map<std::string, std::string>  getHeaders() const;
    std::string                         getQueryString() const;
    std::string                         getAddressClient() const;
    uint16_t                            getPortClient() const;
    bool                                getFormatError() const;
    std::string                         getHttpVersion() const;

    bool                                headerExist(std::string key) const;

    void                                setLocation(std::string);
};

std::ostream &operator <<(std::ostream &, Request &);

#endif
