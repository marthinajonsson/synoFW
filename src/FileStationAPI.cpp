//
// Created by mjonsson on 1/27/19.
//

#include <iostream>
#include "FileStationAPI.h"

void FileStationAPI::load()
{
    Json::Value root;

    std::ifstream json("../api/API_FS", std::ifstream::binary);
    json >> root;
    json.close();

    for (Json::Value::const_iterator its=root.begin(); its!=root.end(); ++its) {
        auto api = *its;
        auto type = api.type();
        auto version = api["maxVersion"].asString();
        auto methods = api["method"].type();
        auto t = 1;
        for (Json::ArrayIndex i = 0; api["method"].isValidIndex(i); i++) {
            auto name = api["method"][i]["name"].asString();
            auto param = api["method"][i]["param"].asString();
            std::cout << name << " " << param << std::endl;
        }
    }
}