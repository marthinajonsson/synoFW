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


std::vector<std::string> VideoStationAPI::respParser(boost::property_tree::ptree &respData, std::string &, std::string &) {

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