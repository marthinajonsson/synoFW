//
// Created by mjonsson on 1/27/19.
//

#ifndef SYNOFW_VIDEOSTATIONAPI_H
#define SYNOFW_VIDEOSTATIONAPI_H

#include <fstream>
#include <string>
#include <map>

#include "RequestHandler.h"
#include "Utilities.h"
#include "API.h"


class VideoStationAPI : public API{
public:
    VideoStationAPI() {
        loadConfig();
    };
    ~VideoStationAPI() = default;

    void makeRequest(std::string&) override;
    std::string paramParser(std::string &,std::string&) override;
    std::vector<std::pair<std::string,std::string>> respParser(boost::property_tree::ptree &, std::string &,std::string&) override;
private:
    std::string _requestUrl;
    const std::string _apiFile = "../api/API_VS";
};


#endif //SYNOFW_VIDEOSTATIONAPI_H
