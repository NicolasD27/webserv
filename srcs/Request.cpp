#include "Request.hpp"

//std::vector<std::string> HEADERS_IN = {"Accept-Language", "Accept", "Keep-Alive", "Authorization", "Via", "Accept-Encoding", "Upgrade", "Expect", "TE", "If-Range", "Range", "Transfer-Encoding", "Content-Type", "Content-Range", "Content-Length", "Referer", "User-Agent", "If-None-Match", "If-Match", "If-Unmodified-Since", "If-Modified-Since", "Connection", "Host"});
const char *HEADERS_IN[] = {"Accept-Language", "Accept", "Keep-Alive", "Authorization", "Via", "Accept-Encoding", "Upgrade", "Expect", "TE", "If-Range", "Range", "Transfer-Encoding", "Content-Type", "Content-Range", "Content-Length", "Referer", "User-Agent", "If-None-Match", "If-Match", "If-Unmodified-Since", "If-Modified-Since", "Connection", "Host", NULL};


Request::Request(std::string request_string) : _request_string(request_string), _body(""), _keep_alive_n(0), _location("")
{
}

std::string & Request::operator[](const char *key) { return _headers[key]; }

void Request::parseHeaders()
{
    std::stringstream buff(_request_string);
    std::string line;
    std::getline(buff, line);
    parseMethod(line);
    while(std::getline(buff, line))
    {
        if (line.length() == 0)
            continue;
        std::string key;
        std::istringstream iss_line(line);        
        if( std::getline(iss_line, key, ':') )
        {
            std::string value;
            if( std::getline(iss_line, value) )
            {
                value = value.substr(1, value.length() - 2);
                storeHeader(key, value);
            }
        }

    }
}

void Request::parseMethod(std::string line)
{
    std::string method;
    std::istringstream iss_line(line);        
    if( std::getline(iss_line, method, ' '))
    {
        _http_method = method;
        std::string location;
        if( std::getline(iss_line, location, ' '))
            handleLocation(location);
    }
}

void Request::handleLocation(std::string location)
{
    int sep_pos = location.find('?');
    if (sep_pos != std::string::npos)
    {
        std::vector<std::string> params_vec = split(location.substr(sep_pos + 1), "&");
        for (std::vector<std::string>::iterator it = params_vec.begin(); it != params_vec.end(); ++it)
        {
            std::vector<std::string> param = split(*it, "=");
            _params.insert(std::make_pair(param[0], urlDecode(param[1])));
        }
        _location = location.substr(0, sep_pos);
    }
    else
        _location = location;
}

void Request::storeHeader(std::string key, std::string value)
{
    int i = 0;
    while (HEADERS_IN[i])
    {
        if (key == std::string(HEADERS_IN[i]) )
        {
           _headers.insert(std::make_pair(key, value));
           break; 
        }
        i++;
    }
    // for (std::vector<std::string>::iterator it = HEADERS_IN.begin(); it != HEADERS_IN.end(); ++it)
    // {
    //     if (key == *it)
    //     {
    //         _headers.insert(std::make_pair(key, value));
    //         break;
    //     }
    // }
}

void Request::addToBody(std::string new_elem) { _body += new_elem;}

void Request::printHeaders(std::ostream & o)
{
    o << C_GREEN << "  Headers" << C_RESET <<" :" << std::endl;
    for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != _headers.end(); ++it)
    {
        o << "\t"<< C_YELLOW << it->first << C_RESET<<": "<< C_CYAN << it->second << C_RESET<< std::endl;
    }
}

void Request::printParams(std::ostream & o)
{
    if (_params.size() != 0)
    {
        o << C_GREEN << "  Params" << C_RESET <<" :" << std::endl;
        for (std::map<std::string, std::string>::iterator it = this->_params.begin(); it != _params.end(); ++it)
        {
            o << "\t"<< C_YELLOW << it->first << C_RESET<<": "<< C_CYAN << it->second << C_RESET<< std::endl;
        }
        
    }
}


std::string        Request::getHttpMethod() const
{
    return this->_http_method;
}

std::string         Request::getLocation() const
{
    return this->_location;
}

void                Request::setLocation(std::string new_loc)
{
    _location = new_loc;
}
time_t              Request::getKeepAliveN() const
{
    return this->_keep_alive_n;
}

std::string         Request::getBody() const
{
    return this->_body;
}

std::map<std::string, std::string>  Request::getParams() const
{
    return this->_params;
}

Request::~Request()
{
}

std::ostream &operator<<(std::ostream & o, Request & request)
{
    o << C_GREEN << "Request:\t"<<C_RED<< request.getHttpMethod() <<C_RESET<<"\n";
    o << C_YELLOW << "\tLocation "<< C_RESET << ": " << request.getLocation() << "\n";
    request.printHeaders(o);
    o << C_YELLOW << "\tKeep alive " << C_RESET << ": " << request.getKeepAliveN()<<"\n";
    o << C_YELLOW << "\tbody " << C_RESET << ": <"<< C_GRAY << request.getBody() << C_RESET <<">\n";
    request.printParams(o);
    return o;
}
