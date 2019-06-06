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
    boost::property_tree::ptree pData;

    if(respData.empty()){
        boost::property_tree::read_json("../api/RequestResponse.json", respData);
    }

    if(api.find("SYNO.FileStation.Info") != std::string::npos) {
        pData = respData.get_child("data");
        auto val = pData.get<std::string>("hostname");
        result.emplace_back(std::make_pair("hostname", val));
        val = pData.get<std::string>("support_sharing");
        result.emplace_back(std::make_pair("support_sharing", val));
        boost::property_tree::write_json(std::cout, respData);
    }else if(api.find("SYNO.FileStation.CreateFolder") != std::string::npos)
    {
        pData = respData.get_child(response);
        BOOST_ASSERT(!pData.empty());

        BOOST_FOREACH(boost::property_tree::ptree::value_type& val , pData) {
            auto name = val.second.get<std::string>("name");
            auto path = val.second.get<std::string>("path");
            result.emplace_back(std::make_pair("name", name));
            result.emplace_back(std::make_pair("path", path));
        }

    }else if(api.find("SYNO.FileStation.Upload") != std::string::npos) {
        result.emplace_back(std::make_pair("Upload", "Complete"));

    }else if(api.find("SYNO.FileStation.Download") != std::string::npos) {
        result.emplace_back(std::make_pair("Download", "Complete"));

    }else if(api.find("SYNO.FileStation.Search") != std::string::npos) {
        for(auto &p : respVec) {
            try {
                if(p.find("files") != std::string::npos) {
                    auto pData = respData.get_child(p);
                    BOOST_ASSERT(!pData.empty());
                    pData = pData.get_child("additional");
                    BOOST_ASSERT(!pData.empty());
                    auto path = pData.get<std::string>("real_path");
                    result.emplace_back(std::make_pair("real_path", path));
                }
                else {
                    auto s = respData.get<std::string>(p);
                    result.emplace_back(std::make_pair(p, s));
                }
            }
            catch(std::exception ex) {
                std::cerr << __FILE__ << " - " << __LINE__ << ": " << ex.what() << std::endl;
            }
        }

    }else if(api.find("SYNO.FileStation.Delete") != std::string::npos) {
        if(respVec.empty()) {
            result.emplace_back(std::make_pair("Delete", "Complete"));
        }else {
            for(auto &p : respVec) {
                auto s = respData.get<std::string>(p);
                result.emplace_back(std::make_pair(p, s));
            }
        }
    } else if(api.find("SYNO.FileStation.List") != std::string::npos) {
        pData = respData.get_child("data");
        for(std::string &p : respVec) {
            try{
                if(p.find("shares") != std::string::npos)
                {
                    auto innerNode = pData.get_child("shares");
                    BOOST_ASSERT(!innerNode.empty());
                    BOOST_FOREACH(boost::property_tree::ptree::value_type& val , innerNode) {
                        auto name = val.second.get<std::string>("name");
                        auto path = val.second.get<std::string>("path");
                        result.emplace_back(std::make_pair("name", name));
                        result.emplace_back(std::make_pair("path", path));
                    }
                }
                else if(p.find("files") != std::string::npos) {
                    auto innerNode = pData.get_child("files");
                    BOOST_ASSERT(!innerNode.empty());
                    BOOST_FOREACH(boost::property_tree::ptree::value_type& val, innerNode) {
                        auto name = val.second.get<std::string>("name");
                        auto path = val.second.get<std::string>("path");
                        result.emplace_back(std::make_pair("name", name));
                        result.emplace_back(std::make_pair("path", path));
                    }
                }
                else if(pData.empty()) {
                    continue;
                }else {
                    auto val = pData.get<std::string>(p);
                    result.emplace_back(std::make_pair(p, val));
                }
            }
            catch (std::exception ex) {
                std::cerr << __FILE__ << " - " << __LINE__ << ": " << ex.what() << std::endl;
            }
        }
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
    auto api = loadAPI(apiFile, parsed);
    auto method = loadMethod(apiFile, api, indexedMethod);
    auto path = loadPath(apiFile, api);
    auto version = loadVersion(apiFile, api);
    auto params = loadParams(apiFile, api, indexedMethod);
    auto resultParams = paramParser(api, params);

    RequestUrlBuilder urlBuilder (info_s.server, path, api, version, method, resultParams);
    auto url = urlBuilder.getResult();

#if DEBUG
    std::cout << url << std::endl;
#endif

    auto responseObject = RequestHandler::getInstance().make(url, "FileStation");

    std::string responses = loadResponse(apiFile, api, indexedMethod);
    std::vector<std::pair<std::string, std::string>> relevant = respParser(responseObject, api, responses);
    for(auto &r : relevant) {
        std::cout << r.first << " : " <<r.second << std::endl;
    }
}

