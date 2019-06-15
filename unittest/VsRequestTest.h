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
    }

    void TearDown() override
    {
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

//TEST_F(VsRequestTest, vsFolder) {
//    std::string input = "folder";
//    vs->makeRequest(input);
//}
//
//TEST_F(VsRequestTest, vsLibrary) {
//    std::string input = "library";
//    vs->makeRequest(input);
//}

//TEST_F(VsRequestTest, vsMeta) {
//   // std::string input = "metadata";
//    //vs->makeRequest(input);
//}



#endif //SYNOFW_VSREQUESTTEST_H
