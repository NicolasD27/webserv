#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

template <typename T>
    std::string NumberToString ( T Number )
    {
        std::ostringstream ss;
        ss << Number;
        return ss.str();
    }

int StringToInt(std::string value);
int StringHexaToInt(std::string value);
int HexaToInt(int value);
std::vector<std::string>    split(std::string const &str, const char *s);
std::string urlEncode(std::string const &);
std::string urlDecode(std::string const &);
std::string	to_string(size_t);
    
#endif