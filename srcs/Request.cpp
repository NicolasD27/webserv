#include "Request.hpp"

//std::vector<std::string> HEADERS_IN = {"Accept-Language", "Accept", "Keep-Alive", "Authorization", "Via", "Accept-Encoding", "Upgrade", "Expect", "TE", "If-Range", "Range", "Transfer-Encoding", "Content-Type", "Content-Range", "Content-Length", "Referer", "User-Agent", "If-None-Match", "If-Match", "If-Unmodified-Since", "If-Modified-Since", "Connection", "Host"});
const char *HEADERS_IN[] = {"Accept-Language", "Accept", "Keep-Alive", "Authorization", "Via", "Accept-Encoding", "Upgrade", "Expect", "TE", "If-Range", "Range", "Transfer-Encoding", "Content-Type", "Content-Range", "Content-Length", "Referer", "User-Agent", "If-None-Match", "If-Match", "If-Unmodified-Since", "If-Modified-Since", "Connection", "Host", NULL};


Request::Request(std::string request_string) : _request_string(request_string), _body(""), _keep_alive_n(0), _location(""), _query_string(""), _format_error(false)
{
}

bool Request::headerExist(std::string key) const { return (_headers.find(key) != _headers.end()); }
std::string & Request::operator[](const char *key) {return _headers[key]; }

void Request::parseHeaders()
{
    std::stringstream buff(_request_string);
    std::string line;
    std::string key;
    std::string value;
    std::getline(buff, line);
    parseMethod(line);
    while(std::getline(buff, line))
    {
        if (line.length() <= 1)
            break;
        line = trim(line);
        std::vector<std::string> tokens = split(line, ":");
        if (tokens.size() == 2)
        {
            key = tokens[0];
            value = trim(tokens[1]);            
            storeHeader(key, value);
        }
        else
        {
            std::cout << "more than 1 ':' in line : " << line << std::endl;
            _format_error = true;
        }
        
    }
    if (_headers.find("Content-Length") != _headers.end() && (StringToInt(_headers["Content-Length"]) < 0 || StringToInt(_headers["Content-Length"]) == 2147483647 || !onlyDigits(_headers["Content-Length"].c_str())))
    {
        std::cout << "content length error" << std::endl;
        _format_error = true;
    }
    

}

void Request::parseMethod(std::string line)
{
    std::string method;
    std::istringstream iss_line(line); 
    line = removeComments(line);
    line = trim(line);
    std::vector<std::string> tokens = split(line, WHITESPACES);
    if (tokens.size() == 3)
    {
        _http_method = tokens[0];
        handleLocation(tokens[1]);
        _http_version = tokens[2];
    }
    else
    {
        std::cout << "wrong number of elements in first line : " << line << std::endl;
        _format_error = true;
    }
    int len = 0;
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
        len += it->length();
    if (len + 2 != line.length())
    {
        std::cout << "space in  first line" << std::endl;
        _format_error = true;
    }
}

void Request::handleLocation(std::string location)
{
    int sep_pos = location.find('?');
    if (sep_pos != std::string::npos)
    {
        _query_string = location.substr(sep_pos+1);
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
        if (key == std::string(HEADERS_IN[i]))
        {
            if (!_headers.insert(std::make_pair(key, value)).second)
                _headers[key] = value;
           break; 
        }
        if (trim(key) == std::string(HEADERS_IN[i])) // il y a des espaces, devrait revonyer code 400
        {
            std::cout << "space in header" << std::endl;
            _format_error = true;
            break;
        }
        i++;
    }
}

void Request::addToBody(std::string new_elem) { _body += new_elem;}

void Request::setPortClient(uint16_t port)
{
    this->_portClient = port;
}

void Request::setAddressClient(std::string &address)
{
    this->_addressClient = address;
}

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

std::string        Request::getHttpVersion() const
{
    return this->_http_version;
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

std::string         Request::getRequestString() const
{
    return this->_request_string;
}

std::string         Request::getQueryString() const
{
    return this->_query_string;
}

std::map<std::string, std::string>  Request::getHeaders() const
{
    return this->_headers;
}

std::string                         Request::getAddressClient() const
{
    return this->_addressClient;
}

uint16_t                            Request::getPortClient() const
{
    return this->_portClient;
}

bool                                Request::getFormatError() const { return _format_error;}

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
