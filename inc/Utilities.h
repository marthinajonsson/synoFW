//
// Created by mjonsson on 1/27/19.
//

#ifndef SYNOFW_UTILITIES_H
#define SYNOFW_UTILITIES_H

#include <vector>

enum API {
    FileStation = 0,
    VideoStation = 1,
};

enum FS_methods {
    Info = 0,
    List,
    Search,
    Download,
    Upload
};



template<typename T>
void pop_front(std::vector<T>& vec)
{
    assert(!vec.empty());
    vec.erase(vec.begin());
}

/*! \struct map_init_helper
* @brief map @Level_e to std::string in @m_levelMap
* @tparam T type of enum
*/
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

/*! \protected map_init
* @brief wrapper for @struct map_init_helper
* @tparam T type of enum
* @param item the enum
* @return type usually std::strings
*/
template<typename T> map_init_helper<T> map_init(T& item)
{
    return map_init_helper<T>(item);
}


#endif //SYNOFW_UTILITIES_H
