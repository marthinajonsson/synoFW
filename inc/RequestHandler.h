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
private:
    struct ConfigInfoS {
        std::string username;
        std::string password;
        std::string server;
    }info_s;

    ConfigHelper _configHelper;
    Logger * pLogger;
    FileLogger * pFileLogger;

    RequestHandler() {
        auto config = _configHelper.getConfig();
        info_s.username = config.username;
        info_s.password = config.password;
        info_s.server = config.server;

        pFileLogger = new FileLogger();
        pLogger = new Logger();
        pLogger->registerObserver(SeverityType::ERROR, pFileLogger);
        pLogger->registerObserver(SeverityType::WARNING, pFileLogger);
#ifdef DEBUG
        std::cout << "DEBUGGING" << std::endl;
        pLogger->registerObserver(SeverityType::GENERAL, pFileLogger);
#endif
    };

    ~RequestHandler() {
        pLogger->removeObserver(pFileLogger);
        delete pFileLogger;
        delete pLogger;
    }

    void login(const std::string&);
    void logoff(const std::string&);
    boost::property_tree::ptree send(const std::string&, std::string &url);
    void sendHttpGetRequest(boost::property_tree::ptree &jsonData, const std::string &url);

public:

    static RequestHandler& getInstance();
    RequestHandler(RequestHandler const&) = delete;
    void operator=(RequestHandler const&) = delete;

    boost::property_tree::ptree make(std::string &url, const std::string& session)
    {
        login(session);
        auto rspObj = send(session, url);
        logoff(session);
        return rspObj;
    }
};


#endif //SYNOFW_REQUESTHANDLER_H
