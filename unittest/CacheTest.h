//
// Created by mjonsson on 5/25/19.
//

#ifndef SYNOFW_CACHETEST_H
#define SYNOFW_CACHETEST_H

#include "include/gmock/gmock.h"
#include "CacheMgr.h"

class CacheTest : public ::testing::Test
{
protected:


    void SetUp() override
    {
        //
    }

    void TearDown() override
    {
        //
    }
};


TEST_F(CacheTest, test1) {
    std::string testFile = "Woman.in.Gold.2015.1080p.BluRay.x264.YIFY.mp4";
    CacheMgr::getInstance().validate(testFile);
    auto db = CacheMgr::getInstance().get(testFile);
    ASSERT_TRUE(db.m_title.find("Woman in Gold") != std::string::npos);
    ASSERT_TRUE(db.m_titleId.find("tt2404425") != std::string::npos);
    ASSERT_TRUE(db.m_genre.find("History") != std::string::npos);
    ASSERT_TRUE(db.m_directors.find("Simon Curtis") != std::string::npos);
    ASSERT_TRUE(db.m_writers.find("Schoenberg") != std::string::npos);
    ASSERT_TRUE(db.m_runtimeMinutes.find("109") != std::string::npos);
}


TEST_F(CacheTest, test2) {
    std::string testFile = "All.That.Heaven.Allows.1955.1080p.BluRay.x264.YIFY.mp4";
    CacheMgr::getInstance().validate(testFile);
    auto db = CacheMgr::getInstance().get(testFile);
    ASSERT_TRUE(db.m_title.find("All That Heaven Allows") != std::string::npos);
    ASSERT_TRUE(db.m_titleId.find("tt0047811") != std::string::npos);
    ASSERT_TRUE(db.m_genre.find("Romance") != std::string::npos);
    ASSERT_TRUE(db.m_directors.find("Douglas Sirk") != std::string::npos);
    ASSERT_FALSE(db.m_runtimeMinutes.find("109") != std::string::npos);
    ASSERT_FALSE(db.m_titleType.find("series") != std::string::npos);
}

#endif //SYNOFW_CACHETEST_H
