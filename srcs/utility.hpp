#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <string>

# define WHITESPACES " \n\r\t\f\v"

template <typename T>
    std::string NumberToString ( T Number )
    {
        std::ostringstream ss;
        ss << Number;
        return ss.str();
    }



int StringToInt(std::string value);
float StringToFloat ( std::string value );
int StringHexaToInt(std::string value);
bool onlyDigits(const char *);
int HexaToInt(int value);
std::vector<std::string>    split(std::string const &str, const char *s);
std::string urlEncode(std::string const &);
std::string urlDecode(std::string const &);
std::string	to_string(size_t);
std::string     removeComments(std::string const &str);
std::string     trim(std::string const &str);
bool compareStringCI(const char * s1, const char * s2);
bool isAlNumHyphen(const char * s);
    
#endif