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


typedef enum SeverityType
{
    ERROR,
    WARNING,
    GENERAL,
}SeverityType;

struct TitleCommon {
    const unsigned short titleId = 0;
};

struct TitleAkas {
    const unsigned short ordering = 1;
    const unsigned short title = 2;
    const unsigned short region = 3;
    const unsigned short language = 4;
};

struct TitleBasics {
    const unsigned short startYear = 5;
    const unsigned short endYear = 6;
    const unsigned short genre = 8;
};

struct TitleCrew {
    const unsigned short directors = 1; //array of string
    const unsigned short writers = 2; //array of string
};

struct TitleEpisode {
    const unsigned short season = 2; //int
    const unsigned short episode = 3; //int
};

struct Names {
    const unsigned short nconst = 0;
    const unsigned short primaryName = 1;
};




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
