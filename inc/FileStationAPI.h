//
// Created by mjonsson on 1/27/19.
//

#ifndef SYNOFW_FILESTATIONAPI_H
#define SYNOFW_FILESTATIONAPI_H


#include <json/json.h>
#include <fstream>
#include <string>
#include <map>

#include "API.h"
#include "Utilities.h"
#include "RequestHandler.h"


class FileStationAPI : API {
public:
    FileStationAPI() {
        loadConfig();
    }
    ~FileStationAPI() = default;

    void makeRequest(std::string&) override;
    std::string loadAPI(std::string&) override;
    std::string loadMethod(std::string&, int&) override;
    std::string loadParams(std::string&, int&) override;
    std::string loadPath(std::string&) override;
    std::string loadVersion(std::string&) override;
    std::string loadResponse(std::string&, int&) override;
    std::string paramParser(std::string &, std::string&) override;
    std::vector<std::string> respParser(Json::Value &, std::string &,std::string&) override;
private:

    std::string requestUrl;
    std::string search_id;
    std::string delete_id;
};


#endif //SYNOFW_FILESTATIONAPI_H
