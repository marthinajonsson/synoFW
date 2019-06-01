//
// Created by mjonsson on 1/27/19.
//

#include <iostream>
#include <algorithm>
#include <boost/assert.hpp>

#include "RequestUrlBuilder.h"
#include "FileStationAPI.h"
#include "ParamHandling.h"
#include "ErrorCodes.h"

std::vector<std::pair<std::string,std::string>> FileStationAPI::respParser(boost::property_tree::ptree &respData, std::string &api, std::string &response) {

    std::vector<std::string> respVec = split(response, ':');
    std::vector<std::pair<std::string,std::string>> result;
    std::string fullRespStr;
    ParamHandling param(testing);
    boost::property_tree::ptree pData, tmp;
    boost::property_tree::read_json("../api/RequestResponse.json", respData);
    pData = respData.get_child("data");
    pData = pData.get_child("object");

    BOOST_FOREACH(boost::property_tree::ptree::value_type& v, pData) {
        auto root = v.second.get_child("");
        BOOST_ASSERT(!root.empty());

        if (root.get<std::string>("type").find("folder") != std::string::npos) {
            continue;
        }
        auto title = root.get<std::string>("title");
        auto path = root.get<std::string>("path");
        auto id = root.get<std::string>("id");
        result.emplace_back(std::make_pair("title", title));
        result.emplace_back(std::make_pair("path", path));
        result.emplace_back(std::make_pair("fileId", id));
        std::cout << "[" << id << "] " << title << std::endl;
    }
    return result;
}

std::string FileStationAPI::paramParser(std::string &api, std::string& params) {

    std::vector<std::string> paramVec = split(params, ':');
    std::string fullParamStr;
    ParamHandling handler(testing);

    for(const std::string &s:paramVec) {
        if(s.empty()) {
            continue;
        }
        fullParamStr += "&";
        fullParamStr += s;
        fullParamStr += "=";

        if(s == "limit") {
            std::string val = handler.setParam("limit", "5");
            fullParamStr+=val;
        }
        else if(s == "sort_by") {
            fullParamStr+="name";
        }
        else if(s == "offset") {
            std::string val = handler.setParam("offset", "0");
            fullParamStr+=val;
        }
        else if(s == "path") {
            std::string val = handler.setParam("path", "film");
            std::string path = handler.getPath(val);
            std::cout << "Path set to: " << path << std::endl;
            fullParamStr+=path;
        }
        else if(s == "folder_path") {
            std::string val = handler.setParam("folder_path", "film");
            auto result = handler.getPathFolder(val);
            fullParamStr+=result;
        }
        else if(s == "pattern"){
            std::string extensions = ".mp4,.mkv,.avi";
            std::string val = handler.setParam("pattern", "");
            std::cout << "Pattern set: " << val << std::endl;
            if(!val.empty()) { val = "," + val; }
            fullParamStr+=extensions+val;
        }
        else if(s == "filetype"){
            std::string val = handler.setParam("filetype", "all");
            fullParamStr+=val;
        }
        else if(s == "extension") {
            std::string val = handler.setParam("extension", "mp4");
        }
        else if(s == "filename") {
            std::string val = handler.setParam("filename", "");
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "File name is required for this operation");
            }
        }
        else if(s == "name") {
            std::string val = handler.setParam("name", "NewFolder");
            fullParamStr+=val;
        }
        else if(s == "create_parents"){
            fullParamStr+="true";
        }
        else if(s == "overwrite"){
            std::cout << "Overwrite? true(default)/false: ";
            std::string val = handler.setParam("overwrite", "true");
            fullParamStr+=val;
        }
        else if(s == "mode") {
            std::cout << "Open in browser? yes write open otherwise download/n: ";
            std::string val = handler.setParam("mode", "open");
            fullParamStr+=val;
        }
        else if(s == "recursive") {
            fullParamStr+="true";
        }
        else if(s == "search_taskid") {
            fullParamStr+=search_id;
        }
        else if(s == "taskid") {
            if(api.find("Search")) {
                fullParamStr+=search_id;
            }else if(api.find("Delete")) {
                fullParamStr+=delete_id;
            }
        }
    }
    return fullParamStr;
}

void FileStationAPI::makeRequest(std::string& parsed) {

    /*
     * info, list, search, create, upload, download, delete
     * */
    int indexedMethod = 0;
    std::string serverUrl = info_s.server;

    auto api = loadAPI(__FILE__, parsed);
    auto method = loadMethod(__FILE__, api, indexedMethod);
    auto path = loadPath(__FILE__, api);
    auto version = loadVersion(__FILE__, api);
    auto params = loadParams(__FILE__, api, indexedMethod);
    auto resultParams = paramParser(api, params);

    RequestUrlBuilder urlBuilder (serverUrl, path, api, version, method, resultParams);
    auto url = urlBuilder.getResult();

#if DEBUG
    std::cout << url << std::endl;
#endif
    auto responseObject = RequestHandler::getInstance().make(url, __FILE__, info_s.username, info_s.password, info_s.server);
    std::string responses = loadResponse(__FILE__, api, indexedMethod);
    respParser(responseObject, api, responses);
}

