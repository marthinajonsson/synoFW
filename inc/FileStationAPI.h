//
// Created by mjonsson on 1/27/19.
//

#ifndef SYNOFW_FILESTATIONAPI_H
#define SYNOFW_FILESTATIONAPI_H

#include <fstream>
#include <string>
#include <map>

#include "API.h"
#include "Utilities.h"
#include "RequestHandler.h"


class FileStationAPI : public API {
public:
    FileStationAPI() {
        loadConfig();
    }
    ~FileStationAPI() = default;

    const std::string _apiFile = "../api/API_FS";
    void makeRequest(std::string&) override;
    std::string paramParser(std::string &, std::string&) override;
    std::vector<std::pair<std::string,std::string>> respParser(boost::property_tree::ptree &, std::string &,std::string&) override;
protected:
    std::string search_id;
    std::string delete_id;
    std::string compile(std::string&, std::string, int, bool);
};


#endif //SYNOFW_FILESTATIONAPI_H
