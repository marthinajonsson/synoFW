//
// Created by mjonsson on 2/3/19.
//

#ifndef SYNOFW_API_H
#define SYNOFW_API_H

#include <string>
#include <fstream>
#include <json/value.h>
#include <cstring>

class API {
public:

    API() = default;
    ~API() = default;

    virtual std::string loadAPI(std::string&) = 0;
    virtual std::string loadMethod(std::string&, int&) = 0;
    virtual std::string loadPath(std::string&) = 0;
    virtual std::string loadVersion(std::string&) = 0;
    virtual std::string loadParams(std::string&, int&) = 0;
    virtual std::string loadResponse(std::string&, int&) = 0;
    virtual std::string paramParser(std::string &,std::string&) = 0;
    virtual std::vector<std::string> respParser(Json::Value &, std::string &,std::string&) = 0;
    virtual void makeRequest(std::string&) = 0;

protected:

    struct Info {
        char username[10];
        char password[10];
        char server[25];
    }info_s;

    void loadConfig() {

        Json::Value root;
        std::ifstream db_read("../config.json", std::ifstream::binary);
        db_read >> root;
        db_read.close();
        strcpy(info_s.username, root["username"].asCString());
        strcpy(info_s.password,root["password"].asCString());
        strcpy(info_s.server, root["server"].asCString());
    }
};

#endif //SYNOFW_API_H
