//
// Created by mjonsson on 5/30/19.
//

#ifndef SYNOFW_FSREQUESTTEST_H
#define SYNOFW_FSREQUESTTEST_H


#include "include/gmock/gmock.h"
#include "FileStationAPI.h"


class FsRequestTest : public ::testing::Test
{
protected:

    std::unique_ptr<FileStationAPI> fs;
    void SetUp() override
    {
        fs = std::make_unique<FileStationAPI>();
        fs->testing = true;

        std::cout << bold_on << "FILE STATION" << bold_off << std::endl;
        std::cout << "fs:info" << "-- get info" << std::endl;
        std::cout << "fs:list" << "-- list shared folders or files" << std::endl;
    }

    void TearDown() override
    {
        fs->testing = false;
    }
};


TEST_F(FsRequestTest, fsInfo) {
    std::string input = "info";
    fs->makeRequest(input);
}

TEST_F(FsRequestTest, fsList) {
    std::string input = "list";
    fs->makeRequest(input);
}


TEST_F(FsRequestTest, fsRespListShare) {
    std::string input = "SYNO.FileStation.List";
    std::string data = "offset:total:shares";
    boost::property_tree::ptree test;
    auto vecPair = fs->respParser(test, input, data);
    int num = 0;
    for(auto &pair : vecPair) {

        num = pair.first.find("offset") != std::string::npos || pair.first.find("total") != std::string::npos ||
                pair.first.find("name") != std::string::npos || pair.first.find("path") != std::string::npos ? ++num : num;
    }
    ASSERT_TRUE(num == 4);
    
}

TEST_F(FsRequestTest, fsRespList) {
    std::string input = "SYNO.FileStation.List";
    std::string data = "offset:total:files";
    boost::property_tree::ptree test;
    auto vecPair = fs->respParser(test, input, data);
    int num = 0;
    for(auto &pair : vecPair) {

        num = pair.first.find("offset") != std::string::npos || pair.first.find("total") != std::string::npos ||
              pair.first.find("name") != std::string::npos || pair.first.find("path") != std::string::npos ? ++num : num;
    }
    ASSERT_TRUE(num == 4);
}
#endif //SYNOFW_FSREQUESTTEST_H
