//
// Created by mjonsson on 1/27/19.
//

#include <iostream>
#include <algorithm>
#include "FileStationAPI.h"

std::string FileStationAPI::loadAPI(std::string &api) {

    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
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

std::string FileStationAPI::loadMethod(std::string& api)
{
    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    auto method = root[api]["method"][0]["name"];
    return method.asString();
}

std::string FileStationAPI::loadParams(std::string &api, std::string &method) {

    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["method"]["param"];
    return params.asString();
}

void FileStationAPI::makeRequest(std::vector<std::string>& parsed) {

}