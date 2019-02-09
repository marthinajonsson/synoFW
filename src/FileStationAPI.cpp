//
// Created by mjonsson on 1/27/19.
//

#include <iostream>
#include <algorithm>
#include "FileStationAPI.h"
#include "ParamHandling.h"
#include "ErrorCodes.h"

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
    throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_API_DOES_NOT_EXISTS, "No API found");
}

std::string FileStationAPI::loadMethod(std::string& api, int&val)
{
    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
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

std::string FileStationAPI::loadParams(std::string &api, int &val) {

    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
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

std::string FileStationAPI::loadPath(std::string& api) {
    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["path"];
    auto result = params.asString();
    if(result.empty()) {
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter API path");
    }
    return result;
}

std::string FileStationAPI::loadVersion(std::string& api) {
    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    auto params = root[api]["maxVersion"];
    auto result = params.asString();
    if(result.empty()) {
        throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "No parameter Version for this API");
    }
    return result;
}

std::string FileStationAPI::loadResponse(std::string &api, int &val) {
    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    auto response = root[api]["method"][val]["response"];
    auto result = response.asString();
    return result;
}

std::vector<std::string> FileStationAPI::respParser(std::string &api, std::string &response) {

    std::vector<std::string> respVec = split(response, ':');
    std::string fullRespStr;
    ParamHandling param;

    for(const std::string &s:respVec)
    {
        if(!s.empty()) {
            fullRespStr += "&";
            fullRespStr += s;
            fullRespStr += "=";
        }

        if(s == "total") {
        }else if(s == "offset"){

        }else if(s == "shares"){
        }
        else if(s == "files"){
        }
        else if(s == "finished"){
        }
        else if(s == "progress"){
        }
        else if(s == "taskid"){
            if(api.find("Search")) {
                search_id = s;
            }else if(api.find("Delete")) {
                delete_id = s;
            }
        }
        else if(s == "folders"){
        }
        else if(s == "path") {
        }
        else if(s == "processing_path"){
        }
    }
    return {""};
}

std::string FileStationAPI::paramParser(std::string &api, std::string& params) {

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
        else if(s == "path" || s == "folder_path") {
            std::string val;
            std::cout << "Choose path" << std::endl;
            std::cin >> val;
            ParamHandling param;
            std::string path = param.getPath(val);
            std::cout << "Path set to: " << path << std::endl;
            fullParamStr+=path;
        }
        else if(s == "pattern"){
            std::string val;
            std::cout << "Choose pattern" << std::endl;
            std::cin >> val;
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                fullParamStr+="*";
            }
        }
        else if(s == "filetype"){
            std::string val;
            std::cout << "Choose file/dir/all(default)" << std::endl;
            std::cin >> val;
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                fullParamStr+="all";
            }
        }
        else if(s == "extension") {
            std::string val;
            std::cout << "Choose file extension" << std::endl;
            std::cin >> val;
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                fullParamStr+="*";
            }
        }
        else if(s == "filename") {
            std::string val;
            std::cout << "Choose filename" << std::endl;
            std::cin >> val;
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "File name is required for this operation");
            }
        }
        else if(s == "name") {
            std::string val;
            std::cout << "Choose name for folder, default NewFolder" << std::endl;
            std::cin >> val;
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                fullParamStr+="NewFolder";
            }
        }
        else if(s == "create_parents"){
            fullParamStr+="true";
        }
        else if(s == "overwrite"){
            std::string val;
            std::cout << "Overwrite? true(default)/false" << std::endl;
            std::cin >> val;
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                fullParamStr+="true";
            }
        }
        else if(s == "mode") {
            std::string val;
            std::cout << "Open in browser? y/n" << std::endl;
            std::cin >> val;
            if(val == "y") {
                fullParamStr+="open";
            }
            else {
                fullParamStr+="download";
            }
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
    RequestHandler::getInstance().make(requestUrl, "FileStation", info_s.username, info_s.password);
}