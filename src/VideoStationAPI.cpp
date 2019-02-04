//
// Created by mjonsson on 1/29/19.
//

//
// Created by mjonsson on 1/27/19.
//

#include <iostream>
#include <algorithm>

#include "VideoStationAPI.h"
#include "Utilities.h"

std::string VideoStationAPI::loadAPI(std::string &api) {

    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    for (Json::Value::const_iterator its=root.begin(); its!=root.end(); ++its) {
        auto key = its.key().asString();
        std::string API = key;

        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        auto found = key.find(api);
        if(found != std::string::npos) {
            return API;
        }
    }
    return "NO_API_FOUND";

}

std::string VideoStationAPI::loadMethod(std::string& api)
{
    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    auto method = root[api]["method"][0]["name"];
    return method.asString();
}

std::string VideoStationAPI::loadParams(std::string &api, std::string &method) {

    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["method"][0]["param"];
    return params.asString();
}

std::string VideoStationAPI::loadPath(std::string& api) {
    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["path"];
    return params.asString();
}

std::string VideoStationAPI::loadVersion(std::string& api) {
    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["maxVersion"];
    return params.asString();
}

std::string VideoStationAPI::paramParser(std::string& params) {
    std::vector<std::string> paramVec = split(params, ':');

    std::string fullParamStr;

    for(const std::string &s:paramVec) {
        fullParamStr += "&";
        fullParamStr += s;
        fullParamStr += "=";
        if(s == "limit") {
            int val;
            std::cout << "Set limit (int): " << std::endl;
            std::cin >> val;
            std::cout << "Limit set to: " << val << std::endl;
            auto valStr = std::to_string(val);
            fullParamStr+=valStr;
        }else if(s == "type") {
            std::string val;
            std::cout << "Set type movie or tvshow (string): " << std::endl;
            std::cin >> val;
            std::cout << "Type set to: " << val << std::endl;
            fullParamStr+=val;
        }
        else if(s == "library_id") {
            fullParamStr+="0";
        }
    }
    return fullParamStr;
}

void VideoStationAPI::makeRequest(std::string& parsed)
{
    /*
     * info, folder, movie, tvshow, library
     * */
    auto API = loadAPI(parsed);
    auto method = loadMethod(API);
    /*
     * get, list
     * */
    auto path = loadPath(API);
    requestUrl+=info_s.server;
    requestUrl+="/webapi/"+path;
    requestUrl+="?api="+API;

    auto version = loadVersion(API);
    auto params = loadParams(API, method);
    requestUrl+="&version="+version;
    requestUrl+="&method="+method;
    auto compiledParam = paramParser(params);
    requestUrl+=compiledParam;
    requestUrl+="&_sid=";
    removeEndOfLines(requestUrl);
    std::cout << requestUrl << std::endl;
    RequestHandler::getInstance().make(requestUrl, "VideoStation", info_s.username, info_s.password);
}