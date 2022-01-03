# webserv

liste des  RFC : RFC 7230, RFC 7231, RFC 7232, RFC 7233, RFC 7234, RFC 7235.

### sites:

https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

https://ncona.com/2019/04/building-a-simple-server-with-cpp/

https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm

### HTTP 1.1 :


http://manpagesfr.free.fr/man/man2/select_tut.2.html


### CGI

http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm

[HTTP/1.1 : Message Syntax and Routing (RFC 7230)](https://www.rfc-editor.org/rfc/rfc7230.html)

[HTTP/1.1 : Semantics and Content (RFC 7231)](https://www.rfc-editor.org/rfc/rfc7231.html)

[HTTP/1.1 : Conditional Requests (RFC 7232)](https://www.rfc-editor.org/rfc/rfc7232.html)

[HTTP/1.1 : Range Requests (RFC 7233)](https://www.rfc-editor.org/rfc/rfc7233.html)

[HTTP/1.1 : Caching (RFC 7234)](https://www.rfc-editor.org/rfc/rfc7234.html)

[HTTP/1.1 : Authentication (RFC 7235)](https://www.rfc-editor.org/rfc/rfc7235.html)

### HTTP Header Syntax

[NGINX Headers struct] http://lxr.nginx.org/source/xref/nginx/src/http/ngx_http_request.h?r=7914%3A9cf043a5d9ca

[HTTP Request Methods](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol#Request_methods)

[HTTP Status Codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)

[HTTP Header Break Style](https://stackoverflow.com/questions/5757290/http-header-line-break-style)
### CGI

http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm

https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm

### Select and non-blocking

[Select](https://www.lowtek.com/sockets/select.html)

[Non-blocking I/O](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm)

### Class: ???

-webserv    : class générale ayant :

            1. une liste de server (voir en dessous)
            2. des fonctions permetant de run, reload, shutdown les serveurs 

- server    : implémente la configuration d'un serveur:  

*attributs* :
            
            -class config      : infos (port, adress, directory du site, directory logs, du cgi etc...)
            -class socket       :

- socket    : stocké dans server ? (regroupant les infos du ou des sockets ):  
*fonctions membres publics* :(hors coplien)  

            ->init() : initialisation  
            ->send() :   
            ->recev():

... to continue...

