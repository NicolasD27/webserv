#include <iostream>
#include <stdlib.h>
#include <string>

#define NB_ENV 16
/*
AUTH_TYPE
CONTENT_LENGTH
CONTENT_TYPE
GATEWAY_INTERFACE
PATH_INFO
PATH_TRANSLATED
QUERY_STRING
REMOTE_ADDR
REMOTE_IDENT
REMOTE_USER
REQUEST_METHOD
REQUEST_URI
SCRIPT_NAME
SERVER_PORT
SERVER_PROTOCOL
SERVER_SOFTWARE

*/

const std::string ENV[ NB_ENV ] = {
   "AUTH_TYPE",
"CONTENT_LENGTH",
"CONTENT_TYPE",
"GATEWAY_INTERFACE",
"PATH_INFO",
"PATH_TRANSLATED",
"QUERY_STRING",
"REMOTE_ADDR",
"REMOTE_IDENT",
"REMOTE_USER",
"REQUEST_METHOD",
"REQUEST_URI",
"SCRIPT_NAME",
"SERVER_PORT",
"SERVER_PROTOCOL",
"SERVER_SOFTWARE"
};   

int main (int argc, char **argv) {

   std::string method="";

  // std::cout << "Content-type:text/html\r\n\r\n";
   std::cout << "<html>\n";
   std::cout << "<head>\n";
   std::cout << "<title>CGI Environment Variables</title>\n";
   std::cout << "</head>\n";
   std::cout << "<body>\n";
   std::cout << "There is " << argc << " args\n";
   for(int i = 0; argv[i]; i++)
      std::cout << "argv["<<i<<"] = "<<argv[i]<<"\n";
   std::cout << "<table border = \"0\" cellspacing = \"2\">";

   for ( int i = 0; i < NB_ENV; i++ ) {
      std::cout << "<tr><td>" << ENV[ i ] << "</td><td>";
      
      // attempt to retrieve value of environment variable
      char *value = getenv( ENV[ i ].c_str() );  
      if ( value != 0 ) {
         std::cout << value;                                 
      } else {
         std::cout << "Environment variable does not exist.";
      }
      std::cout << "</td></tr>\n";
      if(ENV[i] == "REQUEST_METHOD" && value != 0)
      {
         method = std::string(value);
      }
   }
   
   std::cout << "</table>\n";
   if(method == "GET")
   {
      std::string queryString = std::string(getenv("QUERY_STRING"));
      std::cout << "<b>Method GET avec Query_string => '" << queryString << "'</b>"<<std::endl;
   }
   std::cout << "</body>\n";
   std::cout << "</html>\n";
   
   return 0;
}
