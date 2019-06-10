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


TEST_F(VsRequestTest, fsInfo) {
    std::string input = "info";
    vs->makeRequest(input);
}


#endif //SYNOFW_VSREQUESTTEST_H
