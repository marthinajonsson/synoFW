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
                    std::cout << "\t" << nameStr << "\n" << std::endl;
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

    for(const std::string &s:paramVec) {
        if(s.empty()) {
            continue;
        }
        fullParamStr += "&";
        fullParamStr += s;
        fullParamStr += "=";

        if(s == "limit") {
            int val;
            std::cout << "Set limit (int): ";
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
            std::cout << "Set offset (int): ";
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
            std::cout << "Choose path: ";
            std::cin >> val;
            ParamHandling param;
            std::string path = param.getPath(val);
            std::cout << "Path set to: " << path << std::endl;
            fullParamStr+=path;
        }
        else if(s == "folder_path") {
            std::string val;
            ParamHandling param;
            std::cout << "Choose folder path: ";
            std::cin >> val;
            auto result = param.getPathFolder(val);
            fullParamStr+=result;
        }
        else if(s == "pattern"){
            std::string extensions = ".mp4, .mkv, .avi";
            std::string val;
            std::cout << "Filter by pattern: ";
            std::getline(std::cin, val);
            if(!val.empty()) {
                val+=","+extensions;
            }
            else{
                val = extensions;
            }
            std::cout << "Pattern set: " << val << std::endl;
            fullParamStr+=val;
        }
        else if(s == "filetype"){
            std::string val;
            std::cout << "Choose file/dir/all: ";
            std::cin >> val;
            fullParamStr+=val;
            std::cout << "Filetype set" << std::endl;
        }
        else if(s == "extension") {
            std::string val;
            std::cout << "Choose file extension: ";
            std::getline(std::cin, val);
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                fullParamStr+="*";
            }
            std::cout << "Extension set" << std::endl;
        }
        else if(s == "filename") {
            std::string val;
            std::cout << "Choose filename: ";
            std::getline(std::cin, val);
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                throw GENERIC::BadRequestException(GENERIC::ERROR_CODE_NO_PARAMETER, "File name is required for this operation");
            }
            std::cout << "\nFilename set" << std::endl;
        }
        else if(s == "name") {
            std::string val;
            std::cout << "Choose name for folder, default NewFolder: ";
            std::getline(std::cin, val);
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                fullParamStr+="NewFolder";
            }
            std::cout << "\nFolder name set" << std::endl;
        }
        else if(s == "create_parents"){
            fullParamStr+="true";
        }
        else if(s == "overwrite"){
            std::string val;
            std::cout << "Overwrite? true(default)/false: ";
            std::getline(std::cin, val);
            if(!val.empty()) {
                fullParamStr+=val;
            }
            else {
                fullParamStr+="true";
            }
            std::cout << "\n";
        }
        else if(s == "mode") {
            std::string val;
            std::cout << "Open in browser? y/n: ";
            std::getline(std::cin, val);
            if(val == "y") {
                fullParamStr+="open";
            }
            else {
                fullParamStr+="download";
            }
            std::cout << "\nMode set" << std::endl;
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
#if DEBUG
    std::cout << requestUrl << std::endl;
#endif
    auto responseObject = RequestHandler::getInstance().make(requestUrl, "FileStation", info_s.username, info_s.password);
    std::string responses = loadResponse(API, index);
    respParser(responseObject, API, responses);
}

