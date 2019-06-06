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



TEST_F(FsRequestTest, search) {

    std::string input = "search";
    //fs->makeRequest(input);
}

#endif //SYNOFW_FSREQUESTTEST_H
