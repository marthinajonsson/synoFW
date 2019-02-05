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

std::string FileStationAPI::loadMethod(std::string& api, int&val)
{
    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    for (Json::Value::ArrayIndex i = 0; i != root[api]["method"].size(); i++) {
        std::cout << i << ": " << root[api]["method"][i]["name"].asString() << std::endl;
    }

    std::cout << "Choose method: ";
    std::cin >> val;
    auto method = root[api]["method"][val]["name"];
    return method.asString();
}

std::string FileStationAPI::loadParams(std::string &api, int &val) {

    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["method"][val]["param"];
    auto optional = root[api]["method"][val]["optional"];
    return params.asString() + ":" + optional.asString();
}

std::string FileStationAPI::loadPath(std::string& api) {
    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["path"];
    return params.asString();
}

std::string FileStationAPI::loadVersion(std::string& api) {
    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["maxVersion"];
    return params.asString();
}


void FileStationAPI::makeRequest(std::string& parsed) {

}