//
// Created by mjonsson on 1/25/19.
//


#include "RequestHandler.h"

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

std::size_t callback(const char* in, std::size_t size, std::size_t num, std::string* out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}

void RequestHandler::parseData(const Json::Value &data, std::vector<std::string> &argVec) {

    for (Json::Value::const_iterator it=data.begin(); it!=data.end(); ++it) {
        std::string data = it.key().asString() + ':' + it->asString();

        argVec.emplace_back(data);
    }
}

void RequestHandler::parseArgData(const Json::Value &data, std::vector<std::string> &argVec, std::string &&arg)
{
    if(!data.isMember(arg)){
        std::cout << "Key doesn't exists" << std::endl;
        return;
    }

    auto val = data[arg];
    if(val.isArray()) {
        for(const auto &v : val) {
            argVec.emplace_back(v.asString());
        }
    }
    else {
        argVec.emplace_back(val.asString());

    }
}


void RequestHandler::sendHttpGetRequest(Json::Value &jsonData, const std::string &url) {

    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_HEADER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);


    long int httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();
    std::string errors;

    std::cout << httpCode << "\n";

    bool result = reader->parse(httpData.get()->c_str(), httpData->end().base(), &jsonData, &errors);

    delete reader;
}

void RequestHandler::getApiInfo() {
    std::cout << "get API Info.. " << std::endl;
    Json::Value jsonData;
    //std::string url = "http://192.168.0.107:5000/webapi/query.cgi?api=SYNO.API.Info&version=1&method=query&query=all";
    std::string url = "http://192.168.0.107:5000/webapi/query.cgi?api=SYNO.API.Info&version=1&method=query&query=all";
    sendHttpGetRequest(jsonData, url);

    if(!jsonData["success"].asBool()) {
        int errCode = jsonData["error"]["code"].asInt();
        std::string desc = ERROR::COMMON::get(errCode);
        std::cout << "Request failed" << " - " << errCode << " - " << desc << std::endl;
    }

    std::ofstream db_write("../api/Api_Info.json", std::ios::trunc);
    db_write << jsonData["data"];
    db_write.close();

}

void RequestHandler::login(const std::string &session, const std::string& user, const std::string& pwd) {

    std::cout << "LOGIN.. " << std::endl;
    Json::Value jsonData;
    std::string url = "http://192.168.0.107:5000/webapi/auth.cgi?api=SYNO.API.Auth&version=6&method=login&account="+user+"&passwd="+pwd+"&session="+session+"&format=sid";
    removeEndOfLines(url);
    sendHttpGetRequest(jsonData, url);

    if(!jsonData["success"].asBool()) {
        int errCode = jsonData["error"]["code"].asInt();
        std::string desc = ERROR::COMMON::get(errCode);
        std::cout << "Request failed" << " - " << errCode << " - " << desc << std::endl;
    }

    sid = jsonData["data"]["sid"].toStyledString();
}

void RequestHandler::logoff(const std::string &session) {
    std::cout << "LOGOUT.." << std::endl;
    Json::Value jsonData;
    std::string url = "http://192.168.0.107:5000/webapi/auth.cgi?api=SYNO.API.Auth&version=1&method=logout&session="+session;
    removeEndOfLines(url);
    sendHttpGetRequest(jsonData, url);

    sid = "undef";

    if(!jsonData["success"].asBool()) {
        int errCode = jsonData["error"]["code"].asInt();
        std::string desc = ERROR::COMMON::get(errCode);
        std::cout << "Request failed" << " - " << errCode << " - " << desc << std::endl;
    }
}

void RequestHandler::send(std::string &url) {

    Json::Value jsonData;

    if(sid == "undef" || sid.find("null")) {
        return;
    }

    url+=sid;
    removeEndOfLines(url);
    std::cout << url << std::endl;
    sendHttpGetRequest(jsonData, url);
    std::ofstream db_write("../api/API_VideoStation.json", std::ios::trunc);
    db_write << jsonData;
    db_write.close();
//    if(!jsonData["success"].asBool()) {
//        std::cout << "Request failed" << std::endl;
//    }

//    if(!jsonData["success"].asBool()) {
//        int err = jsonData["error"]["code"].asInt();
//        std::string desc;
//        ERROR::get(err, desc);
//        std::cout << "Request failed" << " - " << err << " - " << desc << std::endl;
//    }else {
//        std::ofstream db_write("../api/API_VideoStation.json", std::ios::trunc);
//
//        //   std::ofstream db_write("../api/Api_Info.json", std::ios::trunc);
//        db_write << jsonData;
//        db_write.close();
//        for (Json::Value::const_iterator it=jsonData["data"].begin(); it!=jsonData["data"].end(); ++it)
//        {
//            if(it.key().type() == Json::stringValue){
//                std::cout << it.key().asString() << ":" << *it << std::endl;
//
//            }
//            for(Json::Value::const_iterator it2 = it->begin(); it2 != it->end(); it2++)
//            {
//
//                std::cout << it2.key().asString() << ":" << *it2 << std::endl;
//
//            }
//        }
    //}
}

