#include <sstream>

template <typename T>
    std::string NumberToString ( T Number )
    {
        std::ostringstream ss;
        ss << Number;
        return ss.str();
    }

int StringToInt ( std::string value )
{
    int num;
    if (value.length() == 0)
        return 0;
    std::istringstream ss(value);
    ss >> num;
    return num;
}
    