//
// Created by mjonsson on 2/3/19.
//

#ifndef SYNOFW_API_H
#define SYNOFW_API_H

#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <fstream>
#include <cstring>

#include "ConfigHelper.h"


class API {
public:

    API () = default;
    ~API () = default;

    std::string loadAPI (const std::string &, const std::string&);
    std::string loadMethod (const std::string &file, const std::string&, int&);
    std::string loadMethods (const std::string &file, const std::string&, int&);
    std::string loadPath (const std::string &file, const std::string&);
    std::string loadVersion (const std::string &file, const std::string&);
    std::string loadParams (const std::string &file, const std::string&, int&);
    std::string loadResponse (const std::string &file, const std::string&, const int&);

    virtual void makeRequest (std::string&) = 0;
    virtual std::string paramParser (std::string &,std::string&) = 0;
    virtual std::vector<std::pair<std::string,std::string>> respParser (boost::property_tree::ptree &, std::string &,std::string&) = 0;

protected:

    ConfigHelper m_configHelper;

    struct ConfigInfo {
        std::string username;
        std::string password;
        std::string server;
    }info;

    void loadConfig () {
        auto config = m_configHelper.info;
        info.username = config.username;
        info.password = config.password;
        info.server = config.server;
    }
};

#endif //SYNOFW_API_H
