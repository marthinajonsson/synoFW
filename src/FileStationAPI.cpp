//
// Created by mjonsson on 1/27/19.
//

#include <iostream>
#include <algorithm>
#include "RequestUrlBuilder.h"
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
    val = 0;
    Json::Value method = root[api]["method"];

    for (Json::Value::const_iterator its=method.begin(); its!=method.end(); ++its) {
        auto object = *its;
        auto nameObj = object["name"];
        auto nameStr = nameObj.asString();
        std::cout << i << ": " << nameStr << std::endl;
        i++;
    }

    if(method.size() == 1) {
        std::cout << "Method chosen: " << method[val]["name"].asString() << std::endl;
    }else {
        if(testing) { return method[0]["name"].asString(); }
        std::cout << "Choose method: ";
        std::cin >> val;
    }
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

std::vector<std::string> FileStationAPI::respParser(Json::Value &respData, std::string &api, std::string &response) {

    std::vector<std::string> respVec = split(response, ':');
    std::string fullRespStr;
    ParamHandling param;

    for(const std::string &s:respVec)
    {
        if(s == "taskid"){
            if(api.find("Search")) {
                search_id = s;
            }else if(api.find("Delete")) {
                delete_id = s;
            }
        }
        else if(s == "total" || s == "offset" || s == "finished" || s == "progress" ||
        s == "processing_path" || s == "path")
        {
            try {
                std::cout << s << " : " << respData["data"][s].asString() << std::endl;
            }
            catch (Json::Exception &ex) {
                std::cerr << "Printing string" << ex.what() << std::endl;
            }
        }
        else if(s == "shares" || s == "folders")
        {
            try {
                std::cout << s << " : {\n"  << respData["data"][s] << "\n}" << std::endl;
            }
            catch (Json::Exception &ex) {
                std::cerr << "Printing object" << ex.what() << std::endl;
            }
        }
        else if(s == "files")
        {
            try {
                std::cout << s << " : {\n";
                for (Json::Value::const_iterator its=respData["data"][s].begin(); its!=respData["data"][s].end(); ++its) {
                    auto object = *its;
                    auto nameObj = object["name"];
                    auto nameStr = nameObj.asString();
                    auto found = nameStr.find("vsmeta");
                    if(found != std::string::npos) {
                        continue;
                    }
                    found = nameStr.find(".srt");
                    if(found != std::string::npos){
                        continue;
                    }
                }
                std::cout << " \n}";
            }
            catch (Json::Exception &ex) {
                std::cerr << "Printing object" << ex.what() << std::endl;
            }
        }

    }
    return {""};
}

std::string FileStationAPI::paramParser(std::string &api, std::string& params) {

    std::vector<std::string> paramVec = split(params, ':');
    std::string fullParamStr;
    ParamHandling handler;

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

    auto api = loadAPI(parsed);
    auto method = loadMethod(api, indexedMethod);
    auto path = loadPath(api);
    auto version = loadVersion(api);
    auto params = loadParams(api, indexedMethod);
    auto resultParams = paramParser(api, params);

    RequestUrlBuilder urlBuilder (serverUrl, path, api, version, method, resultParams);
    auto url = urlBuilder.getResult();

#if DEBUG
    std::cout << url << std::endl;
#endif
    auto responseObject = RequestHandler::getInstance().make(url, "FileStation", info_s.username, info_s.password);
    std::string responses = loadResponse(api, indexedMethod);
    respParser(responseObject, api, responses);
}

