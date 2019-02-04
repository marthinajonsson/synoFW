//
// Created by mjonsson on 1/27/19.
//

#ifndef SYNOFW_VIDEOSTATIONAPI_H
#define SYNOFW_VIDEOSTATIONAPI_H

#include <json/json.h>
#include <fstream>
#include <string>
#include <map>

#include "RequestHandler.h"
#include "Utilities.h"
#include "API.h"


class VideoStationAPI : API{
public:
    VideoStationAPI() {
        loadConfig();
    };
    ~VideoStationAPI() = default;

    void makeRequest(std::vector<std::string>&) override;
    std::string loadAPI(std::string&) override;
    std::string loadMethod(std::string&) override;
    std::string loadParams(std::string&, std::string&) override;

private:

    std::string requestUrl;

    void compileUrl(std::string &request) {
        requestUrl+=info_s.server;
        requestUrl+="/webapi/entry.cgi?";
        requestUrl+=request;
    }
};


#endif //SYNOFW_VIDEOSTATIONAPI_H
