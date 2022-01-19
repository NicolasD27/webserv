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

std::vector<std::string>   split(std::string const &str, const char *s)
{
    std::vector<std::string>    ret;
    std::string                 token;
    size_t                      i = 0, pos, len;

    while(i < str.size())
    {
        pos = str.find_first_of(s, i);
        if(pos == std::string::npos)
            break;
        len = pos - i;
        token = str.substr(i, len);
        i += len + 1;
        if(token.empty() == false)
            ret.push_back(token);
    }
    if(i < str.size())
        ret.push_back(str.substr(i));
    return (ret);
}