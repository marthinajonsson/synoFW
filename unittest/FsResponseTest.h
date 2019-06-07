//
// Created by mjonsson on 6/6/19.
//

#ifndef SYNOFW_FSRESPONSETEST_H
#define SYNOFW_FSRESPONSETEST_H


#include "include/gmock/gmock.h"
#include "FileStationAPI.h"

class FsResponseTest : public ::testing::Test
{
protected:

    std::unique_ptr<FileStationAPI> fs;
    boost::property_tree::ptree tmp;
    void SetUp() override
    {
        fs = std::make_unique<FileStationAPI>();
        fs->testing = true;
    }

    void TearDown() override
    {
        fs->testing = false;
    }
};


TEST_F(FsResponseTest, fsInfo) {
    const int methodIndex = 0;
    std::string api = "SYNO.FileStation.Info";
    boost::property_tree::read_json("../api/FS_Info_test.json", tmp);
    std::string responses = fs->loadResponse(fs->_apiFile, api, methodIndex);
    std::vector<std::pair<std::string, std::string>> result = fs->respParser(tmp, api, responses);
    int num = 0;
    for(auto &pair : result) {

        num = pair.first.find("hostname") != std::string::npos || pair.first.find("support_sharing") != std::string::npos
              ? ++num : num;
    }
    ASSERT_TRUE(num == 2);
}

TEST_F(FsResponseTest, fsList) {
    const int methodIndex = 1;
    std::string api = "SYNO.FileStation.List";
    boost::property_tree::read_json("../api/FS_List_List_test.json", tmp);
    std::string responses = fs->loadResponse(fs->_apiFile, api, methodIndex);
    std::vector<std::pair<std::string, std::string>> result = fs->respParser(tmp, api, responses);

    for(auto &r : result) {
        std::cout << r.first << " : " <<r.second << std::endl;
    }

    int num = 0;
    for(auto &pair : result) {

        num = pair.first.find("offset") != std::string::npos || pair.first.find("total") != std::string::npos ||
              pair.first.find("name") != std::string::npos || pair.first.find("path") != std::string::npos ? ++num : num;
    }
    ASSERT_TRUE(num == 4);
}

TEST_F(FsResponseTest, fsListShare) {
    int methodIndex = 0;
    std::string api = "SYNO.FileStation.List";
    boost::property_tree::read_json("../api/FS_List_Share_test.json", tmp);
    std::string responses = fs->loadResponse(fs->_apiFile, api, methodIndex);
    std::vector<std::pair<std::string, std::string>> result = fs->respParser(tmp, api, responses);

    for(auto &r : result) {
        std::cout << r.first << " : " <<r.second << std::endl;
    }
    int num = 0;
    for(auto &pair : result) {

        num = pair.first.find("offset") != std::string::npos || pair.first.find("total") != std::string::npos ||
              pair.first.find("name") != std::string::npos || pair.first.find("path") != std::string::npos ? ++num : num;
    }
    ASSERT_TRUE(num == 8);
}

TEST_F(FsResponseTest, fsCreate) {
    int methodIndex = 0;
    std::string api = "SYNO.FileStation.CreateFolder";
    boost::property_tree::read_json("../api/FS_CreateFolder_Create_test.json", tmp);
    std::string responses = fs->loadResponse(fs->_apiFile, api, methodIndex);
    std::vector<std::pair<std::string, std::string>> result = fs->respParser(tmp, api, responses);

    for(auto &r : result) {
        std::cout << r.first << " : " <<r.second << std::endl;
    }
    int num = 0;
    for(auto &pair : result) {

        num = pair.first.find("name") != std::string::npos || pair.first.find("path") != std::string::npos ? ++num : num;
    }
    ASSERT_TRUE(num == 2);
}

TEST_F(FsResponseTest, fsDelete) {
    int methodIndex = 0;
    std::string api = "SYNO.FileStation.Delete";
    boost::property_tree::read_json("../api/FS_Delete_Start_test.json", tmp);
    std::string responses = fs->loadResponse(fs->_apiFile, api, methodIndex);
    std::vector<std::pair<std::string, std::string>> result = fs->respParser(tmp, api, responses);


    ASSERT_TRUE(result.size() > 0);
    for(auto &r : result) {
        std::cout << r.first << " : " <<r.second << std::endl;
    }
}



#endif //SYNOFW_FSRESPONSETEST_H
