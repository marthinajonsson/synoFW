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


std::vector<std::pair<std::string,std::string>> VideoStationAPI::respParser(boost::property_tree::ptree &respData, std::string &api, std::string &response) {

    std::vector<std::string> respVec = split(response, ':');
    std::vector<std::pair<std::string,std::string>> result;
    std::string fullRespStr;
    ParamHandling param(testing);
    boost::property_tree::ptree pData, tmp;
    boost::property_tree::read_json("../api/RequestResponse.json", respData);
    pData = respData.get_child("data");
    pData = pData.get_child("movie");

    auto title = pData.get<std::string>("title");
    auto path = pData.get<std::string>("path");
    auto id = pData.get<std::string>("id");
    auto mId = pData.get<std::string>("mapper_id");
    result.emplace_back(std::make_pair("title", title));
    result.emplace_back(std::make_pair("path", path));
    result.emplace_back(std::make_pair("fileId", id));
    std::cout << "[" << id << "] " << title << std::endl;

   // BOOST_FOREACH(boost::property_tree::ptree::value_type& root, pData) {
//        auto root = v.second.get_child("");
//        BOOST_ASSERT(!root.empty());
//
//        if (root.get<std::string>("type").find("folder") != std::string::npos) {
//            continue;
//        }

   // }
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
            ParamHandling param(testing);
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
    auto API = loadAPI(__FILE__, parsed);
    int index = 0;
    auto method = loadMethod(__FILE__, API, index);
    /*
     * get, list
     * */
    auto path = loadPath(__FILE__, API);
    requestUrl+=info_s.server;
    requestUrl+="/webapi/"+path;
    requestUrl+="?api="+API;

    auto version = loadVersion(__FILE__, API);
    auto params = loadParams(__FILE__, API, index);
    requestUrl+="&version="+version;
    requestUrl+="&method="+method;
    auto compiledParam = paramParser(API, params);
    requestUrl+=compiledParam;
    requestUrl+="&_sid=";
    removeEndOfLines(requestUrl);

    std::cout << requestUrl << std::endl;
    auto responseObject = RequestHandler::getInstance().make(requestUrl, __FILE__, info_s.username, info_s.password, info_s.server);
}