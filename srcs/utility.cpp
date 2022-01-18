#include "utility.hpp"

int StringToInt ( std::string value )
{
    int num;
    if (value.length() == 0)
        return 0;
    std::istringstream ss(value);
    ss >> num;
    return num;
}

int StringHexaToInt( std::string value)
{
    int y;
    std::stringstream stream(value);

    stream >> std::hex >> y;
    return y;
}

int HexaToInt(int x)
{
    int y;
    std::stringstream stream;

    stream << x;
    stream >> std::hex >> y;
    return y;

}