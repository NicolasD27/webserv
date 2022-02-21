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

float StringToFloat ( std::string value )
{
    float num;
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

bool onlyDigits(const char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] < 48 || str[i] > 57)
            return false;
        i++;
    }
    return true;
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

std::string	to_string(size_t n)
{
	std::stringstream tmp;

	tmp << n;

	return tmp.str();
}
std::string     removeComments(std::string const &str)
{
    std::string     ret = str;
    size_t          posCharComments;

    posCharComments = str.find('#');
    if(posCharComments == std::string::npos)
        return ret;
    return(ret.substr(0, posCharComments));
}

std::string     trim(std::string const &str)
{
    std::string     ret = str;
    size_t          start, end;

    start = ret.find_first_not_of(WHITESPACES);
    if(start != std::string::npos)
        ret = ret.substr(start);
    end = ret.find_last_not_of(WHITESPACES);
    if(end != std::string::npos)
        ret = ret.substr(0, end + 1);
    return ret;
}

bool compareStringCI(const char * s1, const char * s2)
{
    int i = 0;
    if (!s1 && !s2)
        return true;
    if (!s1)
        return false;
    if (!s2)
        return false;
    while (s1[i] && s2[i])
    {
        if (toupper(s1[i]) != toupper(s2[i]))
            return false;
        i++;
    }
    if (s1[i] != s2[i])
        return false;
    return true;
}

bool isAlNumHyphen(const char * s)
{
    int i = 0;
    while (s[i])
    {

        if (!(isalnum(s[i]) || s[i] == '-'))
        {
            std::cout << s[i] + 40 << std::endl;
            return false;
        }
        i++;
    }
    return true;
}