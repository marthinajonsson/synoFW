//
// Created by mjonsson on 1/25/19.
//


#include "RequestHandler.h"

#include <boost/property_tree/ptree.hpp>
#include <ErrorCodes.h>
#include <curl/curl.h>
#include <iostream>
#include <assert.h>
#include <fstream>

std::mutex single;
static std::string FS_sid("undef"); // TODO: handle this in a prettier way
static std::string VS_sid("undef");

static RequestHandler *instance;
RequestHandler& RequestHandler::getInstance()
{
    std::lock_guard<std::mutex> lock(single);
    if(!instance) {
        instance = new RequestHandler();
    }
    return *instance;
}

std::string buffer;
static std::size_t callback(void* in, std::size_t size, std::size_t num, std::string* out)
{
    const std::size_t totalBytes(size * num);
    buffer.append((char*)in, totalBytes);
    return totalBytes;
}

void RequestHandler::sendHttpGetRequest(boost::property_tree::ptree &jsonData, const std::string &url) {

    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_HEADER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);


    long int httpCode(0);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    std::stringstream response(buffer);
    pLogger->writeLog(SeverityType::GENERAL, url);
    pLogger->writeLog(SeverityType::GENERAL, std::to_string(httpCode));
    boost::property_tree::read_json(response, jsonData);
    buffer = "";
}

void RequestHandler::login(const std::string &session)
{
    boost::property_tree::ptree jsonData;
    std::string url = info.server;
    url.append("/webapi/auth.cgi?api=SYNO.API.Auth&version=6&method=login&account="+info.username+"&passwd="+info.password+"&session="+session+"&format=sid");
    removeEndOfLines(url);

    sendHttpGetRequest(jsonData, url);

    auto val = jsonData.get<bool>("success");
    if(!val) {
        boost::property_tree::write_json(std::cerr, jsonData);
        std::string err = "Failed to login to ";
        err.append(session+" with user: " + "{" + info.username + "} and pwd: {" + info.password + "}");
        pLogger->writeLog(SeverityType::ERROR, err);
        GENERIC::printError(jsonData);
    }
    jsonData = jsonData.get_child("data");
    if(session.find("FileStation") != std::string::npos) {
        FS_sid = jsonData.get<std::string>("sid");
    }else if(session.find("VideoStation") != std::string::npos){
        VS_sid = jsonData.get<std::string>("sid");
    }
}

void RequestHandler::logoff(const std::string &session)
{
    boost::property_tree::ptree jsonData;
    std::string url = info.server;
    url.append("/webapi/auth.cgi?api=SYNO.API.Auth&version=1&method=logout&session="+session);
    removeEndOfLines(url);
    sendHttpGetRequest(jsonData, url);

    if(session.find("FileStation") != std::string::npos) {
        FS_sid = "undef";
    }else if(session.find("VideoStation") != std::string::npos){
        VS_sid = "undef";
    }

    auto val = jsonData.get<bool>("success");
    if(!val) {
        boost::property_tree::write_json(std::cerr, jsonData);
        std::string err = "Failed to logoff from ";
        err.append(session);
        pLogger->writeLog(SeverityType::ERROR, err);
        GENERIC::printError(jsonData);
    }
}

boost::property_tree::ptree RequestHandler::send(std::string& url, const std::string &session)
{
    boost::property_tree::ptree jsonData;
    std::string sid = FS_sid;
    if(session.find("VideoStation") != std::string::npos){
        sid = VS_sid;
    }

    try {
        if (sid == "undef" || sid.length() < 5) {
            std::string err = "Sid is either undefined or corrupt: ";
            err.append(url);
            pLogger->writeLog(SeverityType::ERROR, err);
            throw GENERIC::UnhandledRequestException();
        } else {
            url+=sid;
            removeEndOfLines(url);
            sendHttpGetRequest(jsonData, url);
            auto val = jsonData.get<bool>("success");
            if(!val) {
                std::string err = "Failed to send HTTP request: ";
                err.append(session + ", " + url);
                pLogger->writeLog(SeverityType::WARNING, err);
            }
            boost::property_tree::json_parser::write_json("../api/RequestResponse.json", jsonData);
            return jsonData;
        }
    }
    catch (GENERIC::UnhandledRequestException &ex) {
        std::string err = "Failed to send HTTP request URL: ";
        err.append(url + ", " + ex.what());
        pLogger->writeLog(SeverityType::ERROR, err);
        return jsonData;
    }


}

