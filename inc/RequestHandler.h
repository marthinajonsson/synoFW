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



class RequestHandler {
private:
    RequestHandler() = default;
    ~RequestHandler() = default;

    void sendHttpGetRequest(boost::property_tree::ptree &jsonData, const std::string &url);
    boost::property_tree::ptree send(std::string &url);
    void login(const std::string&, const std::string&, const std::string&, const std::string&);
    void logoff(const std::string&, const std::string&);

public:

    static RequestHandler& getInstance();
    RequestHandler(RequestHandler const&) = delete;
    void operator=(RequestHandler const&) = delete;

    boost::property_tree::ptree make(std::string &url, const char* file, const std::string& user, const std::string& pwd, const std::string& server)
    {
        std::string session = strcmp(file, "VideoStation") != 0 ? "VideoStation" : strcmp(file, "FileStation") != 0 ? "FileStation" : "VideoStation";
        login(session, user, pwd, server);
        auto rsp = send(url);
        logoff(session, server);
        return rsp;
    }
};


#endif //SYNOFW_REQUESTHANDLER_H
