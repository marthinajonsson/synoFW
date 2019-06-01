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
    auto vec = fs->respParser(test, input, data);
    auto it = std::find(vec.begin(), vec.end(), "offset");
    ASSERT_TRUE(it != vec.end());
    it = std::find(vec.begin(), vec.end(), "total");
    ASSERT_TRUE(it != vec.end());
    it = std::find(vec.begin(), vec.end(), "name");
    ASSERT_TRUE(it != vec.end());
    it = std::find(vec.begin(), vec.end(), "path");
    ASSERT_TRUE(it != vec.end());
}

TEST_F(FsRequestTest, fsRespList) {
    std::string input = "SYNO.FileStation.List";
    std::string data = "offset:total:files";
    boost::property_tree::ptree test;
    auto vec = fs->respParser(test, input, data);
    auto it = std::find(vec.begin(), vec.end(), "offset");
    ASSERT_TRUE(it != vec.end());
    it = std::find(vec.begin(), vec.end(), "total");
    ASSERT_TRUE(it != vec.end());
    it = std::find(vec.begin(), vec.end(), "name");
    ASSERT_TRUE(it != vec.end());
    it = std::find(vec.begin(), vec.end(), "path");
    ASSERT_TRUE(it != vec.end());
}
#endif //SYNOFW_FSREQUESTTEST_H
