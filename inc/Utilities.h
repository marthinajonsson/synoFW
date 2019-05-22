//
// Created by mjonsson on 1/27/19.
//

#ifndef SYNOFW_UTILITIES_H
#define SYNOFW_UTILITIES_H

#include <vector>
#include <string>
#include <assert.h>
#include <sstream>
#include <memory>
#include <map>


typedef enum SeverityType
{
    ERROR,
    WARNING,
    GENERAL,
}SeverityType;

//metaMap.insert(std::pair<unsigned short, std::string>(common.titleId, "titleId"));
//metaMap.insert(std::pair<unsigned short, std::string>(akas.title, "title"));


static std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}


static std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m";
}


static void removeEndOfLines(std::string &str) {
    std::string::size_type pos = 0;
    while ( ( pos = str.find ("\n",pos) ) != std::string::npos )
    {
        str.erase(pos,2);
    }
}

static std::vector<std::string> split(std::string &input, char delim) {
    std::stringstream m_stream(input);
    std::vector<std::string> seglist;
    std::string segment;
    seglist.reserve(50);
    seglist.clear();

    while(std::getline(m_stream, segment, delim))
    {
        seglist.push_back(std::move(segment));
    }
    return seglist;
}


template<typename T>
void pop_front(std::vector<T>& vec)
{
    assert(!vec.empty());
    vec.erase(vec.begin());
}



template<typename T> struct map_init_helper
{
    T& data;

    explicit map_init_helper(T& d) : data(d) {}
    map_init_helper& operator() (typename T::key_type const& key, typename T::mapped_type const& value)
    {
        data[key] = value;
        return *this;
    }
};


template<typename T> map_init_helper<T> map_init(T& item)
{
    return map_init_helper<T>(item);
}

#endif //SYNOFW_UTILITIES_H
