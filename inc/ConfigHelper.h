//
// Created by mjonsson on 6/1/19.
//

#ifndef SYNOFW_CONFIGHELPER_H
#define SYNOFW_CONFIGHELPER_H

#include <boost/property_tree/json_parser.hpp>

class ConfigHelper {
private:

    struct InfoS {
        std::string username;
        std::string password;
        std::string server;
    }info_s;

public:

    ConfigHelper() {
        boost::property_tree::ptree root;
        boost::property_tree::read_json("../config.json", root);
        info_s.username = root.get<std::string>("username");
        info_s.password = root.get<std::string>("password");
        info_s.server = root.get<std::string>("server");
    }
    ~ConfigHelper() = default;
    InfoS getConfig() { return info_s; }
};
#endif //SYNOFW_CONFIGHELPER_H
