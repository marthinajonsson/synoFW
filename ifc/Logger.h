//
// Created by mjonsson on 5/9/19.
//

#ifndef SYNOFW_LOGGER_H
#define SYNOFW_LOGGER_H

#include <iostream>
#include <map>
#include <ctime>

class Logger {
public:
    /*! \enum Level_e
     *  \brief Defines a the severity of the information that are about to be logged
     *  \var Level_e::INFO inform user of progress, normally no errors
     *  \var Level_e::ERR inform user of error that occured
     *  \var Level_e::WARN warn user that something may be off
     */
    static enum Level_e{ INFO = 0, ERR = 1, WARN = 2 } level;

    /*! \var m_levelMap
     * @brief a map of a key value pair where the key is @Level_e and value a const char*
     */
    std::map<Level_e, const char*> m_levelMap;

    Logger () {
        map_init(m_levelMap)
                (INFO, "[INFO] ")
                (ERR, "[ERROR] ")
                (WARN, "[WARN] ")
                ;
    }
    ~Logger() = default;

    /*! \public TRACE
     * @brief virtual method allowing the user to trace a @Level_e a message and error code to file
     * @param level see @Level_e
     * @param message description of event or progress
     * @param errorCode e.g. @RET::OK
     */
    virtual void TRACE(Level_e level , std::string message, std::string errorCode) = 0;
    /*! \public TRACE
     * @brief virtual method allowing the user to trace a @Level_e a message to file.
     * @param level see @Level_e
     * @param message description of event or progress
     */
    virtual void TRACE(Level_e level , std::string message) = 0;

protected:
    /*! \protected getDate
     * @brief gather and return current date in a std::string
     * @return current date in std::string
     */
    std::string getDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        auto year = 1900 + ltm->tm_year;
        auto month = 1 + ltm->tm_mon;
        auto day = ltm->tm_mday;
        std::string y = std::to_string(year);
        std::string m = std::to_string(month);
        std::string d = std::to_string(day);
        y = y.append("-");
        m = m.append("-");
        return y + m + d;
    }

    /*! \protected getTime
     * @brief gather and return current time in a std::string
     * @return current time in std::string
     */
    std::string getTime() {
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

    /*! \struct map_init_helper
     *@brief map @Level_e to std::string in m_levelMap
     * @tparam T type of enum
     */
    template<typename T> struct map_init_helper
    {
        T& data;
        map_init_helper(T& d) : data(d) {}
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

};


#endif //SYNOFW_LOGGER_H
