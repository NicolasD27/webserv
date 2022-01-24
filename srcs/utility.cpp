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

std::string urlEncode(std::string const &value)
{
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
    {
        std::string::value_type c = (*i);
        if (c == ' ')
        {
            escaped << '+';
            continue;
        }
        // Keep alphanumeric and other accepted characters intact
        if (isalnum((unsigned char) c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char) c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}

std::string urlDecode(std::string const & str)
{
    std::string ret = "";

    for (int i=0; i < str.length(); i++)
    {
        if(str[i] == '%')
        {
            ret.append(1, static_cast<char>(StringHexaToInt(str.substr(i + 1,2))));
            i += 2;
        }
        else
        {
            if(str[i] == '+')
                ret.append(" ");
            else
                ret.append(1, str[i]);
        }
    }
    return ret;
}