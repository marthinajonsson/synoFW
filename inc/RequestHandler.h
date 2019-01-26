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
    void send(std::string &url);
    void login();
    void logoff();


public:

    static RequestHandler& getInstance();
    RequestHandler(RequestHandler const&) = delete;
    void operator=(RequestHandler const&) = delete;

    void make(std::string &url)
    {
        //getApiInfo();
        login();
        send(url);
        logoff();
    }
};


#endif //SYNOFW_REQUESTHANDLER_H
