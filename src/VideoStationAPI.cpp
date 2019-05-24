//
// Created by mjonsson on 1/29/19.
//

//
// Created by mjonsson on 1/27/19.
//

#include <iostream>
#include <algorithm>

#include "VideoStationAPI.h"
#include "ParamHandling.h"
#include "Utilities.h"
#include "ErrorCodes.h"


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
    throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_API_DOES_NOT_EXISTS, "No API found");

}

std::string VideoStationAPI::loadMethod(std::string& api, int& val)
{
    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    int i = 0;
    Json::Value method = root[api]["method"];
    for (Json::Value::const_iterator its=method.begin(); its!=method.end(); ++its) {
        auto object = *its;
        auto nameObj = object["name"];
        auto nameStr = nameObj.asString();
        std::cout << i << ": " << nameStr << std::endl;
        i++;
    }
    std::cout << "Choose method: ";
    std::cin >> val;
    auto result = method[val]["name"].asString();
    if(result.empty()) {
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_METHOD_DOES_NOT_EXISTS, "No API methods found");
    }
    return result;
}

std::string VideoStationAPI::loadParams(std::string &api, int &val) {

    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["method"][val]["param"];
    auto optional = root[api]["method"][val]["optional"];
    auto result = params.asString() + ":" + optional.asString();
    if(result.empty()) {
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter of API method");
    }
    return result;
}

std::string VideoStationAPI::loadPath(std::string& api) {
    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["path"];
    auto result = params.asString();
    if(result.empty()) {
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter API path");
    }
    return result;
}

std::string VideoStationAPI::loadVersion(std::string& api) {
    Json::Value root;

    std::ifstream json("../api/API_VS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["maxVersion"];
    auto result = params.asString();
    if(result.empty()) {
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter Version for this API");
    }
    return result;
}

std::string VideoStationAPI::loadResponse(std::string &api, int &val) {
    return "";
}

std::vector<std::string> VideoStationAPI::respParser(Json::Value &respData, std::string &, std::string &) {

    return {""};
}

std::string VideoStationAPI::paramParser(std::string &api, std::string& params) {

    std::vector<std::string> paramVec = split(params, ':');
    std::string fullParamStr;

    for(const std::string &s:paramVec) {
        if(!s.empty()) {
            fullParamStr += "&";
            fullParamStr += s;
            fullParamStr += "=";
        }
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
        else if(s == "sort_by") {
            fullParamStr+="name";
        }
        else if(s == "offset") {
            int val;
            std::cout << "Set offset (int): " << std::endl;
            std::cin >> val;
            std::cout << "Offset set to: " << val << std::endl;
            if(!val) {
                val = 0;
            }
            auto valStr = std::to_string(val);
            fullParamStr+=valStr;
        }
        else if(s == "path") {
            std::string val;
            std::cout << "Choose path" << std::endl;
            std::cin >> val;
            ParamHandling param;
            std::string path = param.getPath(val);
            std::cout << "Path set to: " << path << std::endl;
            fullParamStr+=path;
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
    int index = 0;
    auto method = loadMethod(API, index);
    /*
     * get, list
     * */
    auto path = loadPath(API);
    requestUrl+=info_s.server;
    requestUrl+="/webapi/"+path;
    requestUrl+="?api="+API;

    auto version = loadVersion(API);
    auto params = loadParams(API, index);
    requestUrl+="&version="+version;
    requestUrl+="&method="+method;
    auto compiledParam = paramParser(API, params);
    requestUrl+=compiledParam;
    requestUrl+="&_sid=";
    removeEndOfLines(requestUrl);

    std::cout << requestUrl << std::endl;
    auto responseObject = RequestHandler::getInstance().make(requestUrl, "VideoStation", info_s.username, info_s.password);
}