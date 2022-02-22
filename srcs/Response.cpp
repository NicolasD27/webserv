/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 22:28:13 by clorin            #+#    #+#             */
/*   Updated: 2022/02/21 15:37:27 by clorin           ###   ########.fr       */
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
    if (handleDots(request.getLocation()))
    {
        std::cout << "dots error" << std::endl;
        _status = 404;
        return;
    }
    findLocation(server, request);
    if (!_location_block)
        _status = 405; // méthode interdite
    else 
        _location_block->print();
    // _cgi_path = server.getCgiPath();
    // buildRessourcePath(request.getLocation(), block);
    std::cout << "_RessourcePath = " << _ressource_path << "\t _status = " << _status << std::endl;
    if(_status == 301)
        _status = 404; 
    else if (_status == 0)
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
    
    if (handleDots(request.getLocation()))
    {
        std::cout << "dots error" << std::endl;
        _status = 404;
        return;
    }
    findLocation(server, request);
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
    if (handleDots(request.getLocation()))
    {
        std::cout << "dots error" << std::endl;
        _status = 404;
        return;
    }
    findLocation(server, request);
    if (!_location_block)
        _status = ( _status == 0) ? 405 : _status; // méthode interdite
    else 
        _location_block->print();
    _cgi_path = server.getCgiPath();
    // buildRessourcePath(request.getLocation(), block);
    std::cout << "_RessourcePath = " << _ressource_path << "\t _status = " << _status << std::endl;
    if (_location_block && _location_block->getRedirectionCode() != 0)
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





bool Response::buildRessourcePath(std::string locRequest, Location const &location)
{
    std::string current_directory = getWorkingPath();
    std::string file_testing;

    _ressource_path = location.getRoot() + "/" + locRequest.substr(location.getPath().length() + ((locRequest.substr(location.getPath().length()).front() == '/') ? 1 : 0));
    _status = 0;
    if (locRequest.back() == '/')
    {
        return findIndex(current_directory, location);
    }
    else
    {   
        // controle si _ressource_path est un directory
        file_testing = current_directory + "/" + _ressource_path;
        std::cout << file_testing<<" is a Directory ? ";
        if(pathIsDir(file_testing))
        {
            std::cout << "Yes\n";
            if (_ressource_path.back() != '/')
                _ressource_path += "/";
            _status = 301;
        }
        else
        {
            std::cout << "No\n";
            
            if(pathIsFile(_ressource_path))
            {
                buildFileFD();
            }
            else
            {

                std::vector<std::string> options = findAlternativeMatches(_ressource_path);
                if (options.size() > 0 && chooseAcceptableFile(options))
                {
                    std::cout << "file found !!" << std::endl;
                    buildFileFD();
                    return false;
                }
                _to_send = true;
                _status = 404;
            }
        }
    }
    return false;
}

bool Response::handleDots(std::string path)
{
    int double_dots = 0;
    int depth = 0;
    int point_pos = 0;
    int i;
    int slash_pos = path.find_first_of('/');

    while ((point_pos = path.find_first_of('.', point_pos)) != std::string::npos)
    {
        if (slash_pos + 1 == point_pos)
        {
            _ressource_path = path;
            return true;
        }
        if ((point_pos + 2 < path.length() && path.substr(point_pos - 1, 4) == "/../") || (path.substr(point_pos - 1, 3) == "/.." && point_pos + 2 == path.length()))
        {
            path = path.substr(0, slash_pos + 1) + path.substr(point_pos + ((point_pos + 2 < path.length()) ? 3 : 2));
            point_pos = slash_pos + 1;
        }
        else
            point_pos += 2;
    }
    _ressource_path = path;
    return false;
}


std::vector<std::string> Response::findAlternativeMatches(std::string current_path)
{
    int last_slash = current_path.find_last_of('/');
    std::vector<std::string> options;
    if (last_slash != std::string::npos)
    {
        std::string relative_path = current_path.substr(0, last_slash + 1);
        std::string abs_path_dir = getWorkingPath() + "/" + relative_path;
        std::cout << "rel path "<< relative_path << std::endl << "abs path " << abs_path_dir << std::endl;
        std::string file = current_path.substr(last_slash + 1);
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (abs_path_dir.c_str())) != NULL)
        {
            while ((ent = readdir (dir)) != NULL)
            {
                std::string alternative_file(ent->d_name);
                if (ent->d_type == DT_REG && alternative_file.length() > file.length()
                    && file == alternative_file.substr(0, file.length()) && alternative_file.at(file.length()) == '.')
                    options.push_back(relative_path + alternative_file);
                
            }
            closedir (dir);
        }
    }
    return options;
}

bool Response::chooseAcceptableFile(std::vector<std::string> options)
{
    MimesType mime_types;
    int fitness;
    int best_fitness;
    float best_quality_type = -1;
    float best_fitness_one_type;
    float best_quality_lang = -1;
    float best_fitness_one_lang;
    float best_quality_one_type;
    float best_quality_one_lang;
    std::string best_match;
    std::string quality;
    std::vector<std::map<std::string, std::string>> type_map = buildTypeMap(options);
    
    // std::vector<std::string> parsed_langs = split(_pt_request->getHeaders()["Accept-Language"], ",");

    std::vector<std::map<std::string, std::string>> target_types = parseAcceptableMIMETypes();
    std::vector<std::map<std::string, std::string>> target_langs = parseAcceptableLanguages();
    
    best_match = "";
    
    for (std::vector<std::map<std::string, std::string>>::iterator it = type_map.begin(); it != type_map.end(); ++it)
    {
        best_fitness_one_type = -1;
        best_quality_one_type = -1;
        for (std::vector<std::map<std::string, std::string>>::iterator ite = target_types.begin(); ite != target_types.end(); ++ite)
        {
            fitness = 0;
            std::cout << "testing : " << it->at("path") << std::endl << it->at("type") << "/" << it->at("subtype") << std::endl;
            std::cout << "with : " << ite->at("type") << "/" << ite->at("subtype") << " q=" << ite->at("q") << std::endl;
            if ((it->at("type") == ite->at("type") || ite->at("type") == "*") && (it->at("subtype") == ite->at("subtype") || ite->at("subtype") == "*"))
            {
                fitness += (it->at("type") == ite->at("type")) ? 100 : 0;
                fitness += (it->at("subtype") == ite->at("subtype")) ? 10 : 0;
                std::cout << "fitness : " << fitness << std::endl;
                if (fitness > best_fitness_one_type)
                {
                    best_fitness_one_type = fitness;
                    best_quality_one_type = StringToFloat(ite->at("q"));
                }
            }
        }
        best_fitness_one_lang = -1;
        best_quality_one_lang = -1;
        for (std::vector<std::map<std::string, std::string>>::iterator ite = target_langs.begin(); ite != target_langs.end(); ++ite)
        {
            fitness = 0;
            std::cout << "testing : " << it->at("path") << std::endl << it->at("lang")  << std::endl;
            std::cout << "with : " << ite->at("lang")  << " q=" << ite->at("q") << std::endl;
            if (it->at("lang") == ite->at("lang") || ite->at("lang") == "*")
            {
                fitness += (it->at("lang") == ite->at("lang")) ? 10 : 0;
                std::cout << "fitness : " << fitness << std::endl;
                if (fitness > best_fitness_one_lang)
                {
                    best_fitness_one_lang = fitness;
                    best_quality_one_lang = StringToFloat(ite->at("q"));
                }
            }
        }
        if (best_quality_one_type > best_quality_type)
        {
            best_quality_type = best_quality_one_type;
            best_quality_lang = best_quality_one_lang;
            best_match = it->at("path");
        }
        else if (best_quality_one_type == best_quality_type)
        {
            if (best_quality_one_lang > best_quality_lang)
            {
                best_quality_lang = best_quality_one_lang;
                best_match = it->at("path");
            }
        }
    }
    if (best_quality_type != -1)
    {
        _ressource_path = best_match;
        return true;
    }
    return false;

}

std::vector<std::map<std::string, std::string>> Response::parseAcceptableMIMETypes()
{
    std::string quality;
    std::vector<std::map<std::string, std::string>> target_types;
    std::vector<std::string> parsed_types = split(_pt_request->getHeaders()["Accept"], ",");

    for (std::vector<std::string>::iterator it = parsed_types.begin(); it != parsed_types.end(); ++it)
    {
        std::map<std::string, std::string> target_type;
        std::vector<std::string> type_quality = split(*it, ";");
        std::vector<std::string> types = split(trim(type_quality.at(0)), "/");
        target_type.insert(std::make_pair("type", types.at(0)));
        target_type.insert(std::make_pair("subtype", types.at(1)));
        if (type_quality.size() >= 2)
        {
            for (std::vector<std::string>::iterator ite = type_quality.begin(); ite != type_quality.end(); ++ite)
            {
                if (trim(*ite)[0] == 'q')
                {
                    std::vector<std::string> params = split(trim(*ite), "=");
                    if (StringToFloat(params.at(1)) < 0 || StringToFloat(params.at(1)) > 1)
                        quality = "1";
                    else
                        quality = trim(params[1]);
                }
            }
            
        }
        else
            quality = "1";
        target_type.insert(std::make_pair("q", quality));
        target_types.push_back(target_type);
    }
    return target_types;
}

std::vector<std::map<std::string, std::string>> Response::parseAcceptableLanguages()
{
    std::string quality;
    std::vector<std::map<std::string, std::string>> target_langs;
    std::vector<std::string> parsed_langs = split(_pt_request->getHeaders()["Accept-Language"], ",");

    for (std::vector<std::string>::iterator it = parsed_langs.begin(); it != parsed_langs.end(); ++it)
    {
        std::map<std::string, std::string> target_lang;
        std::vector<std::string> lang_quality = split(*it, ";");
        target_lang.insert(std::make_pair("lang", lang_quality.at(0)));
        if (lang_quality.size() >= 2)
        {
            for (std::vector<std::string>::iterator ite = lang_quality.begin(); ite != lang_quality.end(); ++ite)
            {
                if (trim(*ite)[0] == 'q')
                {
                    std::vector<std::string> params = split(trim(*ite), "=");
                    if (StringToFloat(params.at(1)) < 0 || StringToFloat(params.at(1)) > 1)
                        quality = "1";
                    else
                        quality = trim(params[1]);
                }
            }
            
        }
        else
            quality = "1";
        target_lang.insert(std::make_pair("q", quality));
        target_langs.push_back(target_lang);
    }
    return target_langs;
}

std::vector<std::map<std::string, std::string>> Response::buildTypeMap(std::vector<std::string> options)
{
    std::vector<std::map<std::string, std::string>> type_map;
    LanguageCodes language_codes;
    MimesType mime_types;
    int point_pos;
    std::string extensions_string;
    std::vector<std::string> extensions;


    for(std::vector<std::string>::iterator it = options.begin(); it != options.end(); ++it)
    {
        std::map<std::string, std::string> file_map;
        file_map.insert(std::make_pair("path", *it));
        point_pos = it->find_first_of('.');
        extensions_string = it->substr(point_pos + 1);
        extensions = split(extensions_string, ".");
        file_map["lang"] = "";
        file_map["type"] = "";
        file_map["subtype"] = "";
        for (std::vector<std::string>::iterator ite = extensions.begin(); ite != extensions.end(); ++ite)
        {
            if (language_codes.langExist(*ite))
                file_map["lang"] = *ite;
            if (mime_types.typeExist(*ite))
            {
                std::vector<std::string> mime_type = split(mime_types.getType(*ite), "/");
                file_map["type"] = mime_type.at(0);
                file_map["subtype"] = mime_type.at(1);
            }

        }
        type_map.push_back(file_map);
    }
    return type_map;
}

bool Response::findIndex(std::string current_directory, Location const &location)
{
    std::string file_testing;

    std::stack<std::string,std::vector<std::string> > index(location.getIndex());
    while (!index.empty())
    {
        file_testing = current_directory + "/" +_ressource_path + index.top();

        std::cout << "Testing : "<< file_testing << " ... ";
        std::ifstream ifs(file_testing);
        if(ifs.good())
        {
            _ressource_path += index.top();
            buildFileFD();
            std::cout << C_GREEN << "Found" << C_RESET << std::endl;
            return true;
        }
        else
        {
            std::cout << C_RED << " Not Found" << C_RESET << std::endl;
            std::vector<std::string> options = findAlternativeMatches(_ressource_path + index.top());
            if (options.size() > 0 && chooseAcceptableFile(options))
            {
                buildFileFD();
                return true;
            }
        }
        index.pop();
    }
    if(location.isAutoindex())
        _status = buildAutoIndex();
    else
        _status = 404;
    return false;
}

void Response::buildFileFD()
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
    delete _pt_request;
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

void Response::findLocation(Server const & server, Request const & request)
{
    std::vector<Location*> locations = server.getLocations();

    
    for (std::vector<Location*>::iterator it = locations.begin(); it != locations.end(); ++it)
    {
        if ((*it)->getPath() == _ressource_path.substr(0, (*it)->getPath().length()))
        {
            if ((*it)->hasMethod(request.getHttpMethod()))
            {
                _location_block = *it;
                if (request.getHttpMethod() == "DELETE")
                    _ressource_path = _location_block->getRoot() + "/" + _ressource_path.substr(_location_block->getPath().length() + ((_ressource_path.substr(_location_block->getPath().length()).front() == '/') ? 1 : 0));
                else if (_location_block->getRedirectionCode() != 0)
                    return;
                else if (buildRessourcePath(_ressource_path, **it))
                    findLocation(server, request);
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