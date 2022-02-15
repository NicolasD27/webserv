/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 22:28:13 by clorin            #+#    #+#             */
/*   Updated: 2022/02/15 14:56:23 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

const char *HEADERS_OUT[] = {"Trailers", "Content-Range", "WWW-Authenticate", "Server", "Date", "Content-Length", "Content-Encoding", "Location", "Last-Modified", "Accept-Ranges", "Expires", "Cache-Control", "ETag", "Content-Type", NULL};

StatusCode STATUS_CODES;

ErrorPages  ERROR_PAGES;

Response::Response(unsigned int status, Server const & server) : _status(status), _to_send(true)
{
    std::cout << "error response" << std::endl;
    buildErrorResponse(server);
}

Response::Response(Request const & request, Server const & server):_pt_server(&server), _pt_request(&request), _location_block(NULL), _body(""), _to_send(true), _ressource_fd(-1), _ressource_path(""), _status(0)
{
    if (request.getHttpVersion() != "HTTP/1.1")
    {
        _status = 505;
        buildErrorResponse(server);
    }
    else if (request.getFormatError())
    {
        _status = 400;
        std::cout << "building error 400..." << std::endl; 
        buildErrorResponse(server);
    }
    else if (request.getHttpMethod() == "GET")
        buildGetResponse(request, server);
    else if (request.getHttpMethod() == "HEAD")
    {
        buildGetResponse(request, server);
        _body = "";
    }
    else if (request.getHttpMethod() == "POST")
        buildPostResponse(request, server);
    else if (request.getHttpMethod() == "DELETE")
        buildDeleteResponse(request, server);   
}
Response::Response(Response const & src)
{
    
    for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
        this->_headers.insert(*it);
    _status = src._status;
    _response_string = src._response_string;
    _ressource_path = src._ressource_path;
    _body = src._body;
    _pt_request = src._pt_request;
    _pt_server = src._pt_server;
    _to_send = src._to_send;
    _ressource_fd = src._ressource_fd;
}

void Response::buildDeleteResponse(Request const & request, Server const & server)
{
    findLocation(request.getLocation(), server, request);
    if (!_location_block)
        _status = 405; // méthode interdite
    else 
        _location_block->print();
    // _cgi_path = server.getCgiPath();
    // buildRessourcePath(request.getLocation(), block);
    std::cout << "_RessourcePath = " << _ressource_path << "\t _status = " << _status << std::endl;
    if(_status == 301)
        _status = 404; 
    if (_status == 0)
    {
        if (!pathIsDir(_ressource_path))
        {
            if(pathIsFile(_ressource_path))
            {
                if (remove(_ressource_path.c_str()) != 0)
                    _status = 500;
                else
                {
                    _body = "<h1>File deleted<h1>";
                    _status = 200;
                }
            }
            else
                _status = 404;
        }
        else
            _status = 404;
    }
    _to_send = true;
    if (_status != 200)
        buildErrorResponse(server);
}

void Response::buildPostResponse(Request const & request, Server const & server)
{
    findLocation(request.getLocation(), server, request);
    if (!_location_block)
        _status = 405; // méthode interdite
    else 
        _location_block->print();
    _cgi_path = server.getCgiPath();
    // buildRessourcePath(request.getLocation(), block);
    std::cout << "_RessourcePath = " << _ressource_path << "\t _status = " << _status << std::endl;
    if(_status == 301)
    {
        _headers.insert(std::make_pair("Content-type", "text/html"));
        _headers.insert(std::make_pair("Location", "http://" + server.getHost() + ":" + NumberToString(server.getPort()) + request.getLocation() + "/"));
    }
    else
    {
        if (_status == 0)
        {
            if (_location_block->hasExtension(_ressource_path)  && _location_block->getCgiPath().length() != 0)
            {
                //CGIHandler  cgi;
                CGIHandler  cgi(_pt_request, this);
                
                std::string     script = _location_block->getCgiPath();

                const char *scriptName[3] = {script.c_str(), _ressource_path.c_str() ,NULL};
                _body = cgi.executeCgi(scriptName,"");
                _status = 200;
            }
        }
        if (_status != 200)
            buildErrorResponse(server);
        else
            parseExtension();
    }
}

void Response::buildGetResponse(Request const & request, Server const & server)
{
    findLocation(request.getLocation(), server, request);
    if (!_location_block)
        _status = 405; // méthode interdite
    else 
        _location_block->print();
    _cgi_path = server.getCgiPath();
    // buildRessourcePath(request.getLocation(), block);
    std::cout << "_RessourcePath = " << _ressource_path << "\t _status = " << _status << std::endl;
    if (_location_block->getRedirectionCode() != 0)
    {
        _status = _location_block->getRedirectionCode();
        std::cout << "redirection" << std::endl;
        // _headers.insert(std::make_pair("Content-type", "text/html"));
        _headers.insert(std::make_pair("Location", _location_block->getRedirectionURL()));
    }
    else if(_status == 301)
    {
        // _headers.insert(std::make_pair("Content-type", "text/html"));
        _headers.insert(std::make_pair("Location", "http://" + server.getHost() + ":" + NumberToString(server.getPort()) + request.getLocation() + "/"));
    }
    else
    {
        if (_status == 0)
        {
            if (!pathIsDir(_ressource_path))
            {
                if(pathIsFile(_ressource_path))
                {
                    _ressource_fd = open(_ressource_path.c_str(), O_NONBLOCK);
                    if (_ressource_fd == -1)
                    {
                        _to_send = true;
                        _status = 403;
                    }
                    else
                    {
                        _to_send = false;
                        _status = 200;
                    }
                }
                else
                {
                    _to_send = true;
                    _status = 404;
                }
            }
            else
                _status = buildAutoIndex();
        }
        if (_status != 200)
            buildErrorResponse(server);
        else
            parseExtension();
    }
}

void Response::buildErrorResponse(Server const & server)
{
    std::map<std::vector<unsigned int>, std::string> error_pages = server.getErrorPages();
    if(error_pages.size() > 0)
    {
        for (std::map<std::vector<unsigned int>, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); ++it)
        {
            for (std::vector<unsigned int>::const_iterator ite = it->first.begin(); ite != it->first.end(); ++ite)
            {
                if (*ite == _status)
                {
                    _ressource_path = it->second;
                    std::cout << "error here " << _ressource_path << std::endl;
                    _headers.insert(std::make_pair("Content-type", "text/html"));
                    _to_send = false;
                    _ressource_fd = open(_ressource_path.c_str(), 0);
                    if (_ressource_fd != -1)
                    {
                        _to_send = true;
                        _body = ERROR_PAGES[_status];
                    }
                    return;
                }
            }
        }
    }
    _body = ERROR_PAGES[_status];
}

std::string Response::buildResponseString()
{
    addDate();
    addLastModifiedDate();
    _headers.insert(std::make_pair("Cache-Control", "no-store"));
    _headers.insert(std::make_pair("Server", "webserv"));
    _headers.insert(std::make_pair("Content-Length", NumberToString(_body.length())));
    _response_string += "HTTP/1.1 " + NumberToString(_status) + " " +  STATUS_CODES[_status] + "\n";
    for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
        _response_string += it->first + ": " + it->second + "\n";
    _response_string += "\n" + _body;
    return _response_string;
}





bool Response::buildRessourcePath(std::string const &locRequest, Location const &location)
{
    std::string current_directory = getWorkingPath();
    std::string file_testing;
    bool        find_index = false;

    _ressource_path = location.getRoot() + "/" + locRequest.substr(location.getPath().length() + ((locRequest.substr(location.getPath().length()).front() == '/') ? 1 : 0));
    _status = 0;
    if (locRequest.back() == '/')
    {
        _headers.insert(std::make_pair("Content-type", "text/html; charset=utf-8"));
        std::stack<std::string,std::vector<std::string> > index(location.getIndex());
        while (!index.empty())
        {
            file_testing = current_directory+"/"+_ressource_path+index.top();

            std::cout << "Testing : "<< file_testing << " ... ";
            std::ifstream ifs(file_testing);
            if(ifs.good())
            {
                _ressource_path += index.top();
                find_index = true;
                std::cout << C_GREEN << "Found" << C_RESET << std::endl;
                break;
            }
            else
            {
                std::cout << C_RED << " Not Found" << C_RESET << std::endl;
            }
            index.pop();
        }
        if(!find_index && !location.isAutoindex())
            _status = 404;
    }
    else
    {   
        // controle si _ressource_path est un directory
        file_testing = current_directory + "/" + _ressource_path;
        std::cout << file_testing<<" is a Directory ? ";
        if(pathIsDir(file_testing))
        {
            std::cout << "Yes\n";
            _ressource_path += "/";
            _status = 301;
        }
        else
            std::cout << "No\n";
    }
    return find_index;
}

void Response::parseExtension()
{
    MimesType MIMES_TYPES;  
    size_t point_pos = _ressource_path.find_last_of('.');
    std::string extension;
    if (point_pos == std::string::npos || point_pos + 1 == _ressource_path.length())
        extension = "application/octet-stream"; // type MIME par défaut dans tous les autres cas. Un fichier de type inconnu doit être associé à ce type MIME. getType() verifie deja donc on peut simplifier
    else
        extension = MIMES_TYPES.getType(this->_ressource_path.substr(point_pos + 1, _ressource_path.length() - point_pos - 1));
        //extension = MIMES_TYPES[_ressource_path.substr(point_pos + 1, _ressource_path.length() - point_pos - 1)];
    _headers.insert(std::make_pair("Content-type", extension));        
}

unsigned int Response::buildAutoIndex()
{
    DIR *dir;
    struct dirent *ent;
    std::string current_directory = getWorkingPath();
    //std::cout << "current_directory au départ = " << current_directory<<"\n";
    //current_directory += "/" + _pt_server->getRoot() + "/" + ((_pt_request->getLocation().length() == 1) ? "" : _pt_request->getLocation());
    current_directory += "/" + _ressource_path;
    std::cout << "current_directory = " << current_directory << "\n";
    if ((dir = opendir (current_directory.c_str())) != NULL)
    {
        _body += "<h1>Index of " + _pt_request->getLocation() + "</h1></br><hr>";
        while ((ent = readdir (dir)) != NULL)
        {
            std::string dir_name(ent->d_name);
            if (ent->d_type == DT_DIR)
                dir_name += "/";
            _body += "<a href=\"" + dir_name + "\" >"+ dir_name + "</a></br>\n" ;
        }
        _body += "<hr>";
        _status = 200;
        closedir (dir);
    }
    else 
    {
        if (pathIsDir(current_directory.c_str()))
            _status = 403;
        else
            _status = 404;
    }
    return _status;
}

unsigned int Response::readRessource(bool isErrorPage)
{

    if (_location_block->hasExtension(_ressource_path)  && _location_block->getCgiPath().length() != 0)
    {
        std::string     script;
        CGIHandler  cgi(_pt_request, this);
        if(_location_block->getCgiPath().at(0) == '/')
            script = _location_block->getCgiPath();
        else
            script = getWorkingPath()+"/"+_location_block->getCgiPath();
        
        const char *scriptName[3] = {script.c_str(), _ressource_path.c_str() ,NULL};
        _body = cgi.executeCgi(scriptName,"");

        return _status;
    }
    std::string str;
    std::stringstream buff;
    int read = false;
    
    std::cout << "ReadRessource ..." << _ressource_path << "...";
    std::ifstream ifs(_ressource_path);
    if(ifs.good() && pathIsFile(_ressource_path))
    {
        while (std::getline(ifs, str))
        {
            read = true;
            buff << str << std::endl;
        }
	    ifs.close();
        if (read)
        {
            _body = buff.str();
            _body.pop_back();
        }
    }
    else if(_status != 200)
        _body = ERROR_PAGES[_status];
    else
    {
        if (pathIsFile(_ressource_path))
            _status = 403;        
        else if (pathIsDir(_ressource_path))
            _status = 301;
        else
            _status = 404;
    }
    std::cout << "_status = " << _status << std::endl;
    return _status;
}

void Response::addDate()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,80,"%a, %d %b %Y %X %Z",timeinfo);
    _headers.insert(std::make_pair("Date", buffer));
}

void Response::addLastModifiedDate()
{
    _headers.insert(std::make_pair("Last-Modified", getLastModifiedDate(_ressource_path)));
}

Response::~Response()
{
}

Response      &Response::operator=(Response const &cpy)
{
    if(this != &cpy)
    {
        _headers = cpy._headers;
        _status = cpy._status;
        _response_string = cpy._response_string;
        _body = cpy._body;
    }
    return *this;
}

Request const * Response::getRequest() const { return _pt_request; }

int         Response::getPort() const { return _pt_server->getPort();}

std::string Response::getHost() const { return _pt_server->getHost();}

std::string Response::getResponseString() const
{
    return _response_string;
}

std::string Response::getRessourcePath() const
{
    return _ressource_path;
}

std::map<std::string, std::string> Response::getHeaders() const
{
    return _headers;
}

std::string             Response::getCgiPath()const
{
    return _location_block->getCgiPath();
}

int Response::getRessourceFD() const { return _ressource_fd; }

bool Response::isToSend() const { return _to_send; }

unsigned int            Response::getStatus() const
{
    return _status;
}

std::string Response::getBody() const { return _body; }

Location * Response::getLocationBlock() const { return _location_block; }

void                    Response::printHeaders(std::ostream & o)
{
    o << C_GREEN << "  Headers" << C_RESET <<" :" << std::endl;
    for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != _headers.end(); ++it)
    {
        o << "\t"<< C_YELLOW << it->first << C_RESET<<": "<< C_CYAN << it->second << C_RESET<< std::endl;
    }
}

void Response::findLocation(std::string const & uri, Server const & server, Request const & request)
{
    std::vector<Location*> locations = server.getLocations();
    
    for (std::vector<Location*>::iterator it = locations.begin(); it != locations.end(); ++it)
    {
        if ((*it)->getPath() == uri.substr(0, (*it)->getPath().length()))
        {
            if ((*it)->hasMethod(request.getHttpMethod()))
            {
                _location_block = *it;
                if (_location_block->getRedirectionCode() != 0)
                    return;
                if (buildRessourcePath(request.getLocation(), **it))
                    findLocation(_ressource_path, server, request);
                return;
            }
        }
    }
}

std::ostream &operator<<(std::ostream & o, Response & response)
{
   
    o << C_GREEN << "Response:\tStatus = "<<C_RED<< STATUS_CODES[response.getStatus()] << " " << response.getStatus() << C_RESET<<"\n";
    o << C_YELLOW << "\tResource path"<< C_RESET << ": " << response.getRessourcePath() << "\n";
    response.printHeaders(o);
    return o;
}