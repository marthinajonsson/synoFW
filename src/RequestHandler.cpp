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
static std::string sid("undef");

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
    boost::property_tree::read_json(response, jsonData);
    buffer = "";
}

void RequestHandler::login(const std::string &session) {

    std::cout << "LOGIN.. " << std::endl;
    boost::property_tree::ptree jsonData;
    std::string url = info_s.server;
    url.append("/webapi/auth.cgi?api=SYNO.API.Auth&version=6&method=login&account="+info_s.username+"&passwd="+info_s.password+"&session="+session+"&format=sid");
    removeEndOfLines(url);

    std::cout << "Sending HTTP request to URL: " << url << std::endl;
    sendHttpGetRequest(jsonData, url);

    auto val = jsonData.get<bool>("success");
    if(!val) {
        GENERIC::printError(jsonData);
    }
    jsonData = jsonData.get_child("data");
    sid = jsonData.get<std::string>("sid");
}

void RequestHandler::logoff(const std::string &session) {
    std::cout << "LOGOUT.." << std::endl;
    boost::property_tree::ptree jsonData;
    std::string url = info_s.server;
    url.append("/webapi/auth.cgi?api=SYNO.API.Auth&version=1&method=logout&session="+session);
    removeEndOfLines(url);
    sendHttpGetRequest(jsonData, url);

    sid = "undef";

    auto val = jsonData.get<bool>("success");
    if(!val) {
        GENERIC::printError(jsonData);
    }
}

boost::property_tree::ptree RequestHandler::send(std::string &url) {

    boost::property_tree::ptree jsonData;

    try {
       if(sid == "undef" || sid.length() < 5) {
           throw GENERIC::UnhandledRequestException();
       }
    }
    catch (GENERIC::UnhandledRequestException &ex) {
        std::cerr << ex.what() << std::endl;
        auto val = jsonData.get<std::string>("success");
        return jsonData;
    }

    url+=sid;
    removeEndOfLines(url);
    std::cout << "Sending HTTP request to URL: " << url << std::endl;
    sendHttpGetRequest(jsonData, url);
    boost::property_tree::json_parser::write_json("../api/RequestResponse.json", jsonData);
    return jsonData;
}

