//
// Created by mjonsson on 5/26/19.
//

#ifndef SYNOFW_FILESTATIONTEST_H
#define SYNOFW_FILESTATIONTEST_H



#include "include/gmock/gmock.h"
#include "FileStationAPI.h"


class FileStationTest : public ::testing::Test
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

/*
 * info, list, search, create, upload, download, delete
 * */

TEST_F(FileStationTest, api) {
    std::string input = "info";

    auto api = fs->loadAPI(__FILE__, input);
    ASSERT_TRUE(api.find("Info") != std::string::npos);

    input = "list";
    api = fs->loadAPI(__FILE__,input);
    ASSERT_TRUE(api.find("List") != std::string::npos);

    input = "search";
    api = fs->loadAPI(__FILE__, input);
    ASSERT_TRUE(api.find("Search") != std::string::npos);

    input = "create";
    api = fs->loadAPI(__FILE__,input);
    ASSERT_TRUE(api.find("Create") != std::string::npos);

    input = "upload";
    api = fs->loadAPI(__FILE__,input);
    ASSERT_TRUE(api.find("Upload") != std::string::npos);

    input = "download";
    api = fs->loadAPI(__FILE__, input);
    ASSERT_TRUE(api.find("Download") != std::string::npos);

    input = "delete";
    api = fs->loadAPI(__FILE__, input);
    ASSERT_TRUE(api.find("Delete") != std::string::npos);
}


TEST_F(FileStationTest, method) {
    std::string api = "SYNO.FileStation.Info";
    int index = 0;

    auto method = fs->loadMethod(__FILE__, api, index);
    ASSERT_TRUE(method.find("get") != std::string::npos);

    api = "SYNO.FileStation.List";
    method = fs->loadMethod(__FILE__, api, index);
    ASSERT_TRUE(method.find("list_share") != std::string::npos);

    api = "SYNO.FileStation.Search";
    method = fs->loadMethod(__FILE__, api, index);
    ASSERT_TRUE(method.find("start") != std::string::npos);

    api = "SYNO.FileStation.CreateFolder";
    method = fs->loadMethod(__FILE__, api, index);
    ASSERT_TRUE(method.find("create") != std::string::npos);

    api = "SYNO.FileStation.Upload";
    method = fs->loadMethod(__FILE__, api, index);
    ASSERT_TRUE(method.find("upload") != std::string::npos);

    api = "SYNO.FileStation.Download";
    method = fs->loadMethod(__FILE__, api, index);
    ASSERT_TRUE(method.find("download") != std::string::npos);

    api = "SYNO.FileStation.Delete";
    method = fs->loadMethod(__FILE__, api, index);
    ASSERT_TRUE(method.find("start") != std::string::npos);
}


TEST_F(FileStationTest, path) {
    std::string input = "SYNO.FileStation.Info";
    auto path = fs->loadPath(__FILE__, input);

    ASSERT_TRUE(path.find("entry.cgi") != std::string::npos);

    input = "SYNO.FileStation.List";
    path = fs->loadPath(__FILE__, input);
    ASSERT_TRUE(path.find("entry.cgi") != std::string::npos);

    input = "SYNO.FileStation.Search";
    path = fs->loadPath(__FILE__, input);
    ASSERT_TRUE(path.find("entry.cgi") != std::string::npos);

    input = "SYNO.FileStation.CreateFolder";
    path = fs->loadPath(__FILE__, input);
    ASSERT_TRUE(path.find("entry.cgi") != std::string::npos);

    input = "SYNO.FileStation.Download";
    path = fs->loadPath(__FILE__, input);
    ASSERT_TRUE(path.find("entry.cgi") != std::string::npos);

    input = "SYNO.FileStation.Upload";
    path = fs->loadPath(__FILE__, input);
    ASSERT_TRUE(path.find("entry.cgi") != std::string::npos);

    input = "SYNO.FileStation.Delete";
    path = fs->loadPath(__FILE__, input);
    ASSERT_TRUE(path.find("entry.cgi") != std::string::npos);
}

TEST_F(FileStationTest, param) {
    std::string input = "SYNO.FileStation.Info";
    int method = 0;
    auto param = fs->loadParams(__FILE__, input, method);

    ASSERT_TRUE(param.find("") != std::string::npos);

    input = "SYNO.FileStation.List";
    method = 1;
    param = fs->loadParams(__FILE__, input, method);
    ASSERT_TRUE(param.find("folder_path") != std::string::npos);

    input = "SYNO.FileStation.Search";
    param = fs->loadParams(__FILE__, input, method);
    ASSERT_TRUE(param.find("taskid") != std::string::npos);

    input = "SYNO.FileStation.CreateFolder";
    method = 0;
    param = fs->loadParams(__FILE__, input, method);
    ASSERT_TRUE(param.find("name") != std::string::npos);

    input = "SYNO.FileStation.Download";
    param = fs->loadParams(__FILE__, input, method);
    ASSERT_TRUE(param.find("path") != std::string::npos);

    input = "SYNO.FileStation.Upload";
    param = fs->loadParams(__FILE__, input, method);
    ASSERT_TRUE(param.find("path") != std::string::npos);

    input = "SYNO.FileStation.Delete";
    param = fs->loadParams(__FILE__, input, method);
    ASSERT_TRUE(param.find("path") != std::string::npos);
}


#endif //SYNOFW_FILESTATIONTEST_H
