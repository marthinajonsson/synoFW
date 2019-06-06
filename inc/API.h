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

    API() = default;
    ~API() = default;

    bool testing = false;
    std::string loadAPI(const std::string &file, const std::string&);
    std::string loadMethod(const std::string &file, const std::string&, int&);
    std::string loadPath(const std::string &file, const std::string&);
    std::string loadVersion(const std::string &file, const std::string&);
    std::string loadParams(const std::string &file, const std::string&, int&);
    std::string loadResponse(const std::string &file, const std::string&, const int&);

    virtual void makeRequest(std::string&) = 0;
    virtual std::string paramParser(std::string &,std::string&) = 0;
    virtual std::vector<std::pair<std::string,std::string>> respParser(boost::property_tree::ptree &, std::string &,std::string&) = 0;

protected:

    ConfigHelper m_configHelper;

    struct ConfigInfoS {
        std::string username;
        std::string password;
        std::string server;
    }info_s;

    void loadConfig() {
        auto config = m_configHelper.getConfig();
        info_s.username = config.username;
        info_s.password = config.password;
        info_s.server = config.server;
    }
};

#endif //SYNOFW_API_H
