#include "Request.hpp"

std::vector<std::string> HEADERS = {"Accept-Language", "Accept", "Keep-Alive", "Authorization", "Via", "Accept-Encoding", "Upgrade", "Expect", "TE", "If-Range", "Range", "Transfer-Encoding", "Content-Type", "Content-Range", "Content-Length", "Referer", "User-Agent", "If-None-Match", "If-Match", "If-Unmodified-Since", "If-Modified-Since", "Connection", "Host"};


Request::Request(std::string request_string) : _request_string(request_string)
{
}

void Request::parseHeaders()
{
    std::stringstream buff(_request_string);
    std::string line;
    std::getline(buff, line);
    getMethod(line);
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
                value = value.substr(1, value.length() - 1);
                storeHeader(key, value);
            }
        }

    }
}

void Request::getMethod(std::string line)
{
    std::string method;
    std::istringstream iss_line(line);        
    if( std::getline(iss_line, method, ' '))
    {
        if (method == "GET")
            _http_method = 1;
        else if (method == "POST")
            _http_method = 2;
        else if (method == "DELETE")
            _http_method = 3;
        else
            _http_method = 0;
        std::string location;
        if( std::getline(iss_line, location, ' '))
            _location = location;
    }
}

void Request::storeHeader(std::string key, std::string value)
{
    for (std::vector<std::string>::iterator it = HEADERS.begin(); it != HEADERS.end(); ++it)
    {
        if (key == *it)
        {
            _headers.insert(std::make_pair(key, value));
            break;
        }
    }
}

void Request::printHeaders(std::ostream & o)
{
    o << C_GREEN << "  Headers" << C_RESET <<" :" << std::endl;
    for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != _headers.end(); ++it)
    {
        o << "\t"<< C_YELLOW << it->first << C_RESET<<": "<< C_CYAN << it->second << C_RESET<< std::endl;
    }
}


unsigned int        Request::get_http_methode() const
{
    return this->_http_method;
}

std::string         Request::get_location() const
{
    return this->_location;
}

time_t              Request::get_keep_alive_n() const
{
    return this->_keep_alive_n;
}

std::string         Request::get_body() const
{
    return this->_body;
}

Request::~Request()
{
}

std::ostream &operator<<(std::ostream & o, Request & request)
{
    const char *Methode[4] = {"Unknown","GET", "POST","DELETE"};
    
    o << C_GREEN << "Request:\t"<<C_RED<<Methode[request.get_http_methode()]<<C_RESET<<"\n";
    o << C_YELLOW << "\tLocation "<< C_RESET << ": " << request.get_location() << "\n";
    o << C_YELLOW << "\tKeep alive " << C_RESET << ": " << request.get_keep_alive_n()<<"\n";
    request.printHeaders(o);
    o << C_YELLOW << "\tbody " << C_RESET << ": <"<< C_GRAY << request.get_body() << C_RESET <<">\n";
    return o;
}
