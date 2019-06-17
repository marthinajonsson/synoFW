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
#include <algorithm>
#include <map>


typedef enum SeverityType
{
    ERROR,
    WARNING,
    GENERAL,
}SeverityType;


static std::ostream& red_on(std::ostream& os)
{
    return os << "\e[31m";
}


static std::ostream& red_off(std::ostream& os)
{
    return os << "\e[37m";
}

static std::ostream& green_on(std::ostream& os)
{
    return os << "\e[32m";
}


static std::ostream& green_off(std::ostream& os)
{
    return os << "\e[37m";
}

static std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}


static std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[21m";
}


static std::ostream& reset(std::ostream& os)
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


static std::string getDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    auto year = 1900 + ltm->tm_year;
    auto month = 1 + ltm->tm_mon;
    std::string y = std::to_string(year);
    std::string m = std::to_string(month);
    y = y.append("-");
    y = y.append(m);
    return y;
}


static std::string getTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int hour = 1 + ltm->tm_hour;
    auto min = 1 + ltm->tm_min;
    auto sec = 1 + ltm->tm_sec;
    std::string h = std::to_string(hour);
    std::string m = std::to_string(min);
    std::string s = std::to_string(sec);
    h = h.append(":");
    m = m.append(":");
    return h + m + s;
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


template <typename Ret, typename Elem>
Ret* getBegin(Elem *e)
{
    return (Elem*)e;
}


template <typename Ret, typename Elem>
Ret* getEnd(Elem *e)
{
    return (Elem*)((Elem*)e+sizeof(*e));
}

#endif //SYNOFW_UTILITIES_H
