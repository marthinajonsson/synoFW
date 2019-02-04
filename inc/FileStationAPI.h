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

    void makeRequest(std::vector<std::string>&) override;
    std::string loadAPI(std::string&) override;
    std::string loadMethod(std::string&) override;
    std::string loadParams(std::string&, std::string&) override;

private:
};


#endif //SYNOFW_FILESTATIONAPI_H
