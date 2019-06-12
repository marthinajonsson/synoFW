//
// Created by mjonsson on 5/30/19.
//

#ifndef SYNOFW_VSREQUESTTEST_H
#define SYNOFW_VSREQUESTTEST_H

#include "include/gmock/gmock.h"
#include "VideoStationAPI.h"


class VsRequestTest : public ::testing::Test
{
protected:

    std::unique_ptr<VideoStationAPI> vs;
    void SetUp() override
    {
        vs = std::make_unique<VideoStationAPI>();
        vs->testing = true;
//        std::cout << bold_on << "VIDEO STATION" << bold_off << std::endl;
//        std::cout << "vs:info" << "-- get info" << std::endl;
//        std::cout << "vs:library" << "-- list all libraries" << std::endl;
//        std::cout << "vs:movie" << std::endl;
//        std::cout << "vs:tvshow" << std::endl;
//        std::cout << "vs:folder" << std::endl;
//        std::cout << "*vs:metadata TBD.." << std::endl;
    }

    void TearDown() override
    {
        vs->testing = false;
    }
};


TEST_F(VsRequestTest, vsInfo) {
    std::string input = "info";
    vs->makeRequest(input);
}

TEST_F(VsRequestTest, vsMovie) {
    std::string input = "movie";
    vs->makeRequest(input);
}

TEST_F(VsRequestTest, vsTVshow) {
    std::string input = "tvshow";
    vs->makeRequest(input);
}

TEST_F(VsRequestTest, vsFolder) {
    std::string input = "folder";
    vs->makeRequest(input);
}

TEST_F(VsRequestTest, vsLibrary) {
    std::string input = "library";
    vs->makeRequest(input);
}

TEST_F(VsRequestTest, vsMeta) {
    std::string input = "metadata";
    vs->makeRequest(input);
}



#endif //SYNOFW_VSREQUESTTEST_H
