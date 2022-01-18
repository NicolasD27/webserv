#include "Response.hpp"

const char *HEADERS_OUT[] = {"Trailers", "Content-Range", "WWW-Authenticate", "Server", "Date", "Content-Length", "Content-Encoding", "Location", "Last-Modified", "Accept-Ranges", "Expires", "Cache-Control", "ETag", "Content-Type", NULL};

StatusCode STATUS_CODES;

ErrorPages  ERROR_PAGES;

Response::Response(Request const & request, Server const & server):_pt_server(&server), _pt_request(&request), _body(""), _to_send(true), _ressource_fd(-1)
{
    Location    block;
    std::cout << "Creation de Response avec Request.getLocation() = " << request.getLocation();
    block = findLocation(request.getLocation(), server);
    std::cout << "on travail avec le block : \n";
    block.print();

    buildRessourcePath(request.getLocation(), block);
    std::cout << "_RessourcePath = " << _ressource_path << "\t _status = " << _status << std::endl;
    if(_status == 301)
    {
        _headers.insert(std::make_pair("Content-type", "text/html"));
        _headers.insert(std::make_pair("Location", "http://" + server.getHost() + ":" + NumberToString(server.getPort()) + request.getLocation() + "/"));
        // std::cout << "http://" << server.getHost() << ":" << NumberToString(server.getPort()) << request.getLocation() << "/" << std::endl;
    }
    else if(_status == 404)
    {
        std::map<std::vector<unsigned int>, std::string> error_pages = server.getErrorPages();
        if(error_pages.size() > 0)
        {
            for (std::map<std::vector<unsigned int>, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); ++it)
                for (std::vector<unsigned int>::const_iterator ite = it->first.begin(); ite != it->first.end(); ++ite)
                {
                    if (*ite == _status)
                    {
                        _ressource_path = server.getRoot() + it->second;
                        _headers.insert(std::make_pair("Content-type", "text/html"));
                        _to_send = false;
                        _ressource_fd = open(_ressource_path.c_str(), O_NONBLOCK);
                        if (_ressource_fd == -1)
                        {
                            _body = ERROR_PAGES[_status];
                            _to_send = true;
                        }
                    }
                    break;
                }
        }
        else
            _body = ERROR_PAGES[_status];
    }
    else
    {
        
        if (!pathIsDir(_ressource_path))
        {
            
            _ressource_fd = open(_ressource_path.c_str(), O_NONBLOCK);
            if (_ressource_fd == -1)
            {
                _to_send = true;
                _status = 404;
            }
            else
            {
                _to_send = false;
                _status = 200;
            }

        }
            // _status = readRessource(false);
        else
            _status = buildAutoIndex();
        
        if (_status != 200)
        {
            std::map<std::vector<unsigned int>, std::string> error_pages = server.getErrorPages();
            if(error_pages.size() > 0)
            {
            for (std::map<std::vector<unsigned int>, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); ++it)
                for (std::vector<unsigned int>::const_iterator ite = it->first.begin(); ite != it->first.end(); ++ite)
                {
                    if (*ite == _status)
                    {
                        _ressource_path = server.getRoot() + it->second;
                        _headers.insert(std::make_pair("Content-type", "text/html"));
                        _to_send = false;
                        _ressource_fd = open(_ressource_path.c_str(), O_NONBLOCK);
                        if (_ressource_fd == -1)
                        {
                            _body = ERROR_PAGES[_status];
                            _to_send = true;
                        }
                    }
                    break;
                }
            }
            else
                _body = ERROR_PAGES[_status];
        }
        else
            parseExtension();
    }
}

std::string Response::buildResponseString()
{
    addDate();
    _headers.insert(std::make_pair("Cache-Control", "no-store"));
    _headers.insert(std::make_pair("Server", "webserv"));
    _headers.insert(std::make_pair("Content-Length", NumberToString(_body.length())));
    _response_string += "HTTP/1.1 " + NumberToString(_status) + " " +  STATUS_CODES[_status] + "\n";
    for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
        _response_string += it->first + ": " + it->second + "\n";
    _response_string += "\n" + _body;
    return _response_string;
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



void Response::buildRessourcePath(std::string const &locRequest, Location const &location)
{
    std::string current_directory = getWorkingPath();
    std::string file_testing;
    bool        find_index = false;

    _ressource_path = location.getRoot() + locRequest;
    _status = 0;
    if (_ressource_path.back() == '/')
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
    std::string str;
    std::stringstream buff;
    
    std::cout << "ReadRessource ..." << _ressource_path << "...";
    std::ifstream ifs(_ressource_path);
    if(ifs.good() && pathIsFile(_ressource_path))
    {
        if(!isErrorPage)
            _status = 200;

        while (std::getline(ifs, str))
            buff << str << std::endl;
	    ifs.close();
        _body = buff.str();
    }
    else if(isErrorPage)
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

std::string Response::getResponseString() const
{
    return _response_string;
}

std::string Response::getRessourcePath() const
{
    return _ressource_path;
}

int Response::getRessourceFD() const { return _ressource_fd; }

bool Response::isToSend() const { return _to_send; }

unsigned int            Response::getStatus() const
{
    return this->_status;
}

void                    Response::printHeaders(std::ostream & o)
{
    o << C_GREEN << "  Headers" << C_RESET <<" :" << std::endl;
    for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != _headers.end(); ++it)
    {
        o << "\t"<< C_YELLOW << it->first << C_RESET<<": "<< C_CYAN << it->second << C_RESET<< std::endl;
    }
}

Location                Response::findLocation(std::string const &uri, Server const &server)
{
    std::vector<Location> loc = server.getLocation();
    std::cout << "\nfind the Location request : " << uri << " in Location of server\n";

    size_t i = 0;
    size_t pos_start = uri.find_first_not_of('/',1);
    size_t pos_end = uri.find_first_of('/', pos_start);

    std::string str = "/";
    if(pos_end != std::string::npos)
        str = uri.substr(0,pos_end);

    for(; i < loc.size(); i++)
    {
        std::cout << "\t"<<i<<") "<< loc[i].getPath() << " == " << str << " ... ";
        if(loc[i].getPath() == str)
        {
            std::cout << C_GREEN << " Found" << C_RESET << std::endl;
            return loc[i];   
        }
        else
            std::cout << C_RED << " No" << C_RESET << std::endl;
    }
    return loc.back();
}

std::ostream &operator<<(std::ostream & o, Response & response)
{
   
    o << C_GREEN << "Response:\tStatus = "<<C_RED<< STATUS_CODES[response.getStatus()] << " " << response.getStatus() << C_RESET<<"\n";
    o << C_YELLOW << "\tResource path"<< C_RESET << ": " << response.getRessourcePath() << "\n";
    response.printHeaders(o);
    return o;
}