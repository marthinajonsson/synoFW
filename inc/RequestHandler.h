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
#include <Logger.h>

#include "ConfigHelper.h"
#include "FileLogger.h"
#include "Utilities.h"

class RequestHandler {
public:

    static RequestHandler& getInstance();
    RequestHandler(RequestHandler const&) = delete;
    void operator=(RequestHandler const&) = delete;

    boost::property_tree::ptree make(std::string& url, const std::string& session)
    {
        login(session);
        auto rspObj = send(url, session);
        logoff(session);
        return rspObj;
    }
private:
    struct ConfigInfo {
        std::string username;
        std::string password;
        std::string server;
    }info;

    ConfigHelper _configHelper;
    Logger * pLogger;
    FileLogger * pFileLogger;

    RequestHandler () {
        auto config = _configHelper.info;
        info.username = config.username;
        info.password = config.password;
        info.server = config.server;

        pFileLogger = new FileLogger();
        pLogger = new Logger();
        pLogger->registerObserver(SeverityType::ERROR, pFileLogger);
        pLogger->registerObserver(SeverityType::WARNING, pFileLogger);
#ifdef BOOST_PREPROCESSOR_DEBUG_ERROR_HPP
        std::cout << "DEBUGGING" << std::endl;
        pLogger->registerObserver(SeverityType::GENERAL, pFileLogger);
#endif
    };

    ~RequestHandler () {
        pLogger->removeObserver(pFileLogger);
        delete pFileLogger;
        delete pLogger;
    }

    void login (const std::string&);
    void logoff (const std::string&);
    boost::property_tree::ptree send (std::string&, const std::string &);
    void sendHttpGetRequest (boost::property_tree::ptree &jsonData, const std::string &url);
};


#endif //SYNOFW_REQUESTHANDLER_H
