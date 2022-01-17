#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <sstream>

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
    
#endif