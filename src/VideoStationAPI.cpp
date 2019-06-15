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

std::vector<std::pair<std::string,std::string>> VideoStationAPI::respParser (boost::property_tree::ptree &respData, std::string &api, std::string &response)
{
    boost::property_tree::ptree pData;
    boost::property_tree::ptree tmp;
    std::vector<std::string> respVec = split (response, ':');
    std::vector<std::pair<std::string,std::string>> result;
    std::string fullRespStr;
    ParamHandling param(true);

    if (respData.empty())
        boost::property_tree::read_json("../api/RequestResponse.json", respData);

    if (api.find("SYNO.VideoStation2.Info") != std::string::npos) {
        pData = respData.get_child("data");
        auto val = pData.get<std::string>("is_subtitle_search_enabled");
        result.emplace_back(std::make_pair("is_subtitle_search_enabled", val));
        val = pData.get<std::string>("version_string");
        result.emplace_back(std::make_pair("version_string", val));
    }
    else if (api.find("SYNO.VideoStation2.Movie") != std::string::npos) {
        pData = respData.get_child("data");
        for (std::string &p : respVec) {
            try {
                if (p.find("movie") != std::string::npos) {
                    auto innerNode = pData.get_child("movie");
                    BOOST_ASSERT(!innerNode.empty());
                    BOOST_FOREACH(boost::property_tree::ptree::value_type &val, innerNode) {
                                    auto title = val.second.get<std::string>("title");
                                    auto tagline = val.second.get<std::string>("tagline");
                                    auto id = val.second.get<std::string>("id");
                                    auto library_id = val.second.get<std::string>("library_id");
                                    auto mapper_id = val.second.get<std::string>("mapper_id");
                                    result.emplace_back(std::make_pair("title", title));
                                    result.emplace_back(std::make_pair("tagline", tagline));
                                    result.emplace_back(std::make_pair("id", id));
                                    result.emplace_back(std::make_pair("library_id", library_id));
                                    result.emplace_back(std::make_pair("mapper_id", mapper_id));
                                }
                } else if (pData.empty()) {
                    continue;
                } else {
                    auto val = pData.get<std::string>(p);
                    result.emplace_back(std::make_pair(p, val));
                }
            }
            catch (std::exception ex) {
                std::cerr << __FILE__ << " - " << __LINE__ << ": " << ex.what() << std::endl;
            }
        }
    }else  if (api.find("SYNO.VideoStation2.TVShow") != std::string::npos) {
        pData = respData.get_child("data");
        for (std::string &p : respVec) {
            try {
                if (p.find("tvshow") != std::string::npos) {
                    auto innerNode = pData.get_child("tvshow");
                    BOOST_ASSERT(!innerNode.empty());
                    BOOST_FOREACH(boost::property_tree::ptree::value_type &val, innerNode) {
                                    auto title = val.second.get<std::string>("title");
                                    auto id = val.second.get<std::string>("id");
                                    auto library_id = val.second.get<std::string>("library_id");
                                    auto mapper_id = val.second.get<std::string>("mapper_id");
                                    result.emplace_back(std::make_pair("title", title));
                                    result.emplace_back(std::make_pair("id", id));
                                    result.emplace_back(std::make_pair("library_id", library_id));
                                    result.emplace_back(std::make_pair("mapper_id", mapper_id));
                                    auto additional = val.second.get_child("additional");
                                    BOOST_ASSERT(!additional.empty());
                                    auto season = additional.get<std::string>("total_seasons");
                                    result.emplace_back(std::make_pair("total_seasons", season));

                                }
                } else if (pData.empty()) {
                    continue;
                } else {
                    auto val = pData.get<std::string>(p);
                    result.emplace_back(std::make_pair(p, val));
                }
            }
            catch (std::exception ex) {
                std::cerr << __FILE__ << " - " << __LINE__ << ": " << ex.what() << std::endl;
            }
        }
    }
    else {
        boost::property_tree::write_json(std::cout, respData);
    }

    return result;
}

std::string VideoStationAPI::paramParser(std::string &api, std::string& params) {

    std::vector<std::string> paramVec = split(params, ':');
    std::string fullParamStr;
    ParamHandling handler(true);

    for(const std::string &s:paramVec) {
        if(!s.empty()) {
            fullParamStr += "&";
            fullParamStr += s;
            fullParamStr += "=";
        }
        if(s == "limit") {
            std::string val = handler.setParam("limit", "5");
            fullParamStr+=val;
        }else if(s == "type") {
            std::string val = handler.setParam("type", "movie");
            fullParamStr+=val;
        }
        else if(s == "library_id") {
            fullParamStr+="0";
        }
        else if(s == "sort_by") {
            fullParamStr+="title";
        }
        else if(s == "offset") {
            std::string val = handler.setParam("offset", "0");
            fullParamStr+=val;
        }
        else if(s == "path") {
            std::string val = handler.setParam("path", "Rom");
            std::string path = handler.getPath(val);
            fullParamStr+=path;
        }
    }
    return fullParamStr;
}

std::string VideoStationAPI::compile(std::string &input, std::string &api, int indexedMethod = 0, bool chooseMethod = true)
{
    api = loadAPI(_apiFile, input);
    std::string method;

    if(!chooseMethod)
        method = loadMethod(_apiFile, api, indexedMethod);
    else
        method = loadMethods(_apiFile, api, indexedMethod);


    auto path = loadPath(_apiFile, api);
    auto version = loadVersion(_apiFile, api);
    auto params = loadParams(_apiFile, api, indexedMethod);
    auto resultParams = paramParser(api, params);

    RequestUrlBuilder urlBuilder (info.server, path, api, version, method, resultParams);
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

    for(auto &p : result) {
        std::cout << p.first << ":" << p.second << std::endl;
    }
}