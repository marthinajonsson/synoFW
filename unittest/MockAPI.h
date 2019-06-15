//
// Created by mjonsson on 6/15/19.
//

#ifndef SYNOFW_MOCKAPI_H
#define SYNOFW_MOCKAPI_H

#include "API.h"
#include <vector>
#include <gmock/gmock-spec-builders.h>

class MockAPI : public API, public ::testing::Mock {
public:
    MockAPI () {};
    ~MockAPI () {};

    MOCK_METHOD3 (loadMethod, std::string(const std::string &file, const std::string& api, int& val));
    MOCK_METHOD1 (makeRequest, void(std::string &file));
    MOCK_METHOD2 (paramParser, std::string(std::string &,std::string&));
    MOCK_METHOD3 (respParser, std::vector<std::pair<std::string,std::string>>(boost::property_tree::ptree &, std::string &,std::string&));
};

#endif //SYNOFW_MOCKAPI_H
