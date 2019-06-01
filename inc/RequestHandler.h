//
// Created by mjonsson on 1/25/19.
//

#ifndef SYNOFW_REQUESTHANDLER_H
#define SYNOFW_REQUESTHANDLER_H

#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>

#include "ConfigHelper.h"

class RequestHandler {
private:
    struct ConfigInfoS {
        std::string username;
        std::string password;
        std::string server;
    }info_s;

    ConfigHelper m_configHelper;

    RequestHandler() {
        auto config = m_configHelper.getConfig();
        info_s.username = config.username;
        info_s.password = config.password;
        info_s.server = config.server;
    };

    ~RequestHandler() = default;

    void login(const std::string&);
    void logoff(const std::string&);
    boost::property_tree::ptree send(std::string &url);
    void sendHttpGetRequest(boost::property_tree::ptree &jsonData, const std::string &url);

public:

    static RequestHandler& getInstance();
    RequestHandler(RequestHandler const&) = delete;
    void operator=(RequestHandler const&) = delete;

    boost::property_tree::ptree make(std::string &url, std::string&& session)
    {
        login(session);
        auto rsp = send(url);
        logoff(session);
        return rsp;
    }
};


#endif //SYNOFW_REQUESTHANDLER_H
