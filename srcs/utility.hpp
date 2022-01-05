#include <sstream>

template <typename T>
    std::string NumberToString ( T Number )
    {
        std::ostringstream ss;
        ss << Number;
        return ss.str();
    }