//
// Created by mjonsson on 6/1/19.
//

#ifndef SYNOFW_CONFIGHELPER_H
#define SYNOFW_CONFIGHELPER_H

#include <boost/property_tree/json_parser.hpp>

class ConfigHelper {
private:
    struct Info {
        std::string username;
        std::string password;
        std::string server;
    };
public:
    Info info;
    ConfigHelper() {
        boost::property_tree::ptree root;
        boost::property_tree::read_json("../config.json", root);
        info.username = root.get<std::string>("username");
        info.password = root.get<std::string>("password");
        info.server = root.get<std::string>("server");
    }
    ~ConfigHelper() = default;
};
#endif //SYNOFW_CONFIGHELPER_H
