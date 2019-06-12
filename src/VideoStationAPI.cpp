//
// Created by mjonsson on 1/29/19.
//

//
// Created by mjonsson on 1/27/19.
//

#include <iostream>
#include <algorithm>

#include <RequestUrlBuilder.h>
#include "VideoStationAPI.h"
#include "ParamHandling.h"
#include "Utilities.h"
#include "ErrorCodes.h"
#include "HttpRequests.h"

std::vector<std::pair<std::string,std::string>> VideoStationAPI::respParser(boost::property_tree::ptree &respData, std::string &api, std::string &response) {

    std::vector<std::string> respVec = split(response, ':');
    std::vector<std::pair<std::string,std::string>> result;
    std::string fullRespStr;
    ParamHandling param(testing);
    boost::property_tree::ptree pData, tmp;
    //sort_by=name not working
//    pData = respData.get_child("data");
    boost::property_tree::write_json(std::cout, respData);
//    pData = pData.get_child("movie");
//
//    auto title = pData.get<std::string>("title");
//    auto path = pData.get<std::string>("path");
//    auto id = pData.get<std::string>("id");
//    auto mId = pData.get<std::string>("mapper_id");
//    result.emplace_back(std::make_pair("title", title));
//    result.emplace_back(std::make_pair("path", path));
//    result.emplace_back(std::make_pair("fileId", id));
//    std::cout << "[" << id << "] " << title << std::endl;
    return result;
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
            fullParamStr+="title";
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
            ParamHandling param(testing);
            std::string path = param.getPath(val);
            std::cout << "Path set to: " << path << std::endl;
            fullParamStr+=path;
        }
    }
    return fullParamStr;
}

std::string VideoStationAPI::compile(std::string &input, std::string &api, int indexedMethod = 0, bool chooseMethod = true)
{
    api = loadAPI(_apiFile, input);
    chooseMethod = testing ? false : chooseMethod;
    auto method = loadMethod(_apiFile, api, indexedMethod, std::move(chooseMethod));
    auto path = loadPath(_apiFile, api);
    auto version = loadVersion(_apiFile, api);
    auto params = loadParams(_apiFile, api, indexedMethod);
    auto resultParams = paramParser(api, params);

    RequestUrlBuilder urlBuilder (info_s.server, path, api, version, method, resultParams);
    return urlBuilder.getResult();
}


void VideoStationAPI::makeRequest(std::string& parsed)
{
    /*
     * info, folder, movie, tvshow, library
     * */
    std::vector<std::pair<std::string,std::string>> result;
    std::string api;
    int indexedMethod = 0;
    std::string url = compile(parsed, api, indexedMethod, true);
    auto responseObject = RequestHandler::getInstance().make(url, VideoStation::session);
    std::string responses = loadResponse(_apiFile, api, indexedMethod);
    result = respParser(responseObject, api, responses);
}