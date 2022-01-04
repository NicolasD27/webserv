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

void Request::printHeaders()
{
    std::cout << "Headers :" << std::endl;
    for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}



Request::~Request()
{
}
