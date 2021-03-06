//
// Created by mjonsson on 1/27/19.
//

#include <boost/thread/thread.hpp>
#include <boost/assert.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <algorithm>

#include "RequestUrlBuilder.h"
#include "FileStationAPI.h"
#include "ParamHandling.h"
#include "ErrorCodes.h"
#include <HttpRequests.h>
#include <TablePrinter.h>
#include <CacheMgr.h>


bool operator==( const std::pair<std::string, std::string>& lhs,
                 const std::pair<std::string, std::string>& rhs ) {
    if(lhs.second.find("name") != std::string::npos) {
        return true;
    }
    else if(rhs.second.find("name") != std::string::npos) {
        return true;
    }
    return false;
}

std::vector<std::pair<std::string,std::string>> FileStationAPI::respParser (boost::property_tree::ptree &respData, std::string &api, std::string &response)
{
    std::vector<std::string> respVec = split(response, ':');
    std::vector<std::pair<std::string,std::string>> result;
    std::string fullRespStr;
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
        pData = respData.get_child("data");
        for(auto &p : respVec) {
            try {
                if(p.find("files") != std::string::npos) {
                    pData = pData.get_child(p);
                    if(pData.empty()){
                        boost::property_tree::write_json(std::cout, pData);
                        continue;
                    }
                    BOOST_FOREACH(boost::property_tree::ptree::value_type& val , pData) {
                            auto name = val.second.get<std::string>("name");
                            auto path = val.second.get<std::string>("path");
                            result.emplace_back(std::make_pair("name", name));
                            result.emplace_back(std::make_pair("path", path));
                    }
                }
                else {
                    auto s = pData.get<std::string>(p);
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

std::string FileStationAPI::paramParser (std::string &api, std::string& params)
{
    std::vector<std::string> paramVec = split (params, ':');
    std::string fullParamStr;
    ParamHandling handler (true);

    for (const std::string &s:paramVec) {
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
            std::string val = handler.setParam("path", "Rom");
            std::string path = handler.getPath(val);
            std::cout << "Path set to: " << path << std::endl;
            fullParamStr+=path;
        }
        else if(s == "folder_path") {
            std::string val = handler.setParam("folder_path", "Rom");
            auto result = handler.getPathFolder(val);
            fullParamStr+=result;
        }
        else if(s == "pattern"){
            //std::string extensions = ".mp4,.mkv,.avi";
            std::string val = handler.setParam("pattern", "The");
            std::cout << "Pattern set: " << val << std::endl;
            //if(!val.empty()) { val = "," + val; }
            fullParamStr+=val;
        }
        else if(s == "filetype"){
            std::string val = handler.setParam("filetype", "all");
            fullParamStr+=val;
        }
        else if(s == "extension") {
            std::string val = handler.setParam("extension", "mp4");
            fullParamStr+=val;
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

std::string FileStationAPI::compile (std::string &input, std::string &api, int indexedMethod = 0, bool chooseMethod = true)
{
    api = loadAPI (_apiFile, input);

    std::string method;
    if (!chooseMethod)
        method = loadMethod (_apiFile, api, indexedMethod);
    else
        method = loadMethods (_apiFile, api, indexedMethod);
    auto path = loadPath (_apiFile, api);
    auto version = loadVersion (_apiFile, api);
    auto params = loadParams (_apiFile, api, indexedMethod);
    auto resultParams = paramParser (api, params);

    RequestUrlBuilder urlBuilder (info.server, path, api, version, method, resultParams);
    return urlBuilder.getResult();
}

void FileStationAPI::makeRequest (std::string& parsed)
{

    /*
     * info, list, search, create, upload, download, delete
     * */


#ifdef TEST_RUNNING
    std::cout << "TEST DEFINED" << std::endl;
#endif
    std::vector<std::pair<std::string,std::string>> result;

    std::string api;
    if (parsed.find("search") != std::string::npos)
    {
        int val = 0;

        std::cout << "Start a a search (0) or clean cache (1)" << std::endl;
        std::cin >> val;

        if (val == 0) {
            std::string urlStart = compile(parsed, api, 0, false);

            auto responseObject = RequestHandler::getInstance().make(urlStart, FileStation::session);
            std::string responses = loadResponse(_apiFile, api, 0);
            result = respParser(responseObject, api, responses);
            BOOST_ASSERT(!result.empty());
            auto pair = result.front();
            search_id = pair.second;

            boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));
            std::string urlStop = compile(parsed, api, 2, false);

            RequestHandler::getInstance().make(urlStop, FileStation::session);

            std::string urlList = compile(parsed, api, 1, false);
            responseObject = RequestHandler::getInstance().make(urlList, FileStation::session);
            responses = loadResponse(_apiFile, api, 1);
            result = respParser(responseObject, api, responses);
            BOOST_ASSERT(!result.empty());
            for(auto &res : result) {
                std::cout << res.first << ": " << res.second << std::endl;
            }
        } else if (val == 1) {
            std::string urlClean = compile(parsed, api, 3, false);
            RequestHandler::getInstance().make(urlClean, FileStation::session);
            search_id = "";
        }


    }else if (parsed.find("delete") != std::string::npos) {

        std::string urlDelete = compile(parsed, api, 0, false);

        auto responseObject = RequestHandler::getInstance().make(urlDelete, FileStation::session);
        std::string responses = loadResponse(_apiFile, api, 3);
        result = respParser(responseObject, api, responses);
        BOOST_ASSERT(!result.empty());
        auto pair = result.front();
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    else {
        int indexedMethod = 0;
        std::string url = compile(parsed, api, indexedMethod, true);
        auto responseObject = RequestHandler::getInstance().make(url, FileStation::session);
        std::string responses = loadResponse(_apiFile, api, indexedMethod);
        result = respParser(responseObject, api, responses);
    }

    TablePrinter tablePrinter(6);
    tablePrinter.setRowColor("bold");
    tablePrinter.addHeader({"Index","Title", "Genre", "Duration", "Directors", "Actors"});
    tablePrinter.setRowColor("");

    auto it = result.begin();
    Database obj;

    for (;it != result.end(); it++) {
        if (it->first.find("name") != std::string::npos) {
            CacheMgr::getInstance().validate(it->second);
            obj = CacheMgr::getInstance().get(it->second);
        }

        if (it->first.find("path") != std::string::npos)
            obj.m_path = it->second;

        CacheMgr::getInstance().edit(obj);
        tablePrinter.addRow({obj.m_title, obj.m_genre, obj.m_runtimeMinutes, obj.m_directors, obj.m_writers});
    }

    tablePrinter.write();
}

