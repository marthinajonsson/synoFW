//
// Created by mjonsson on 1/29/19.
//

//
// Created by mjonsson on 1/27/19.
//

#include <iostream>
#include <algorithm>
#include "VideoStationAPI.h"

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

    auto method = root[api]["method"]["name"];
    return method.asString();
}

std::string VideoStationAPI::loadParams(std::string &api, std::string &method) {

    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["method"]["param"];
    return params.asString();
}

void VideoStationAPI::makeRequest(std::vector<std::string>& parsed)
{
    std::string api = parsed.front();
    pop_front(parsed);
    loadAPI(api);

}