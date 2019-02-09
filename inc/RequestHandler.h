//
// Created by mjonsson on 1/25/19.
//

#ifndef SYNOFW_REQUESTHANDLER_H
#define SYNOFW_REQUESTHANDLER_H

#include <json/json.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>



class RequestHandler {
private:
    RequestHandler() = default;
    ~RequestHandler() = default;

    void parseArgData(const Json::Value &data, std::vector<std::string> &argVec, std::string &&arg);
    void parseData(const Json::Value &data, std::vector<std::string> &argVec);


    void sendHttpGetRequest(Json::Value &jsonData, const std::string &url);
    void getApiInfo();
    Json::Value send(std::string &url);
    void login(const std::string&, const std::string&, const std::string&);
    void logoff(const std::string&);

public:

    static RequestHandler& getInstance();
    RequestHandler(RequestHandler const&) = delete;
    void operator=(RequestHandler const&) = delete;

    Json::Value make(std::string &url, std::string&& session, const std::string& user, const std::string& pwd)
    {
        login(session, user, pwd);
        auto rsp = send(url);
        logoff(session);
        return rsp;
    }
};


#endif //SYNOFW_REQUESTHANDLER_H
