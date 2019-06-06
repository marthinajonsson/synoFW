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

    //auto responseObject = RequestHandler::getInstance().make(url, "FileStation");
    std::string input = "search";
    fs->makeRequest(input);
   // "http://192.168.0.107:5000/webapi/entry.cgi?api=SYNO.FileStation.Search&version=2&method=start&folder_path=/video/film&pattern=.mp4,.mkv,.avi&extension=&filetype=all&_sid="
}

#endif //SYNOFW_FSREQUESTTEST_H
