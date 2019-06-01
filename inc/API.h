//
// Created by mjonsson on 2/3/19.
//

#ifndef SYNOFW_API_H
#define SYNOFW_API_H

#include <string>
#include <fstream>
#include <cstring>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>


class API {
public:

    API() = default;
    ~API() = default;

    bool testing = false;
    std::string loadAPI(const char*, std::string&);
    std::string loadMethod(const char*, std::string&, int&);
    std::string loadPath(const char*, std::string&);
    std::string loadVersion(const char*, std::string&);
    std::string loadParams(const char*, std::string&, int&);
    std::string loadResponse(const char*, std::string&, int&);

    virtual std::string paramParser(std::string &,std::string&) = 0;
    virtual std::vector<std::pair<std::string,std::string>> respParser(boost::property_tree::ptree &, std::string &,std::string&) = 0;
    virtual void makeRequest(std::string&) = 0;

protected:

    struct Info {
        char username[10];
        char password[10];
        char server[25];
    }info_s;

    void loadConfig() {

        boost::property_tree::ptree root;
        boost::property_tree::read_json("../config.json", root);
        auto val = root.get<std::string>("username");
        strcpy(info_s.username, val.c_str());
        val = root.get<std::string>("password");
        strcpy(info_s.password, val.c_str());
        val = root.get<std::string>("server");
        strcpy(info_s.server, val.c_str());
    }
};

#endif //SYNOFW_API_H
