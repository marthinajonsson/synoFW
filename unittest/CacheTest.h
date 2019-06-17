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
    std::map<PROPERTY, std::string> _propertyMap;

    void SetUp() override
    {
        map_init(_propertyMap)
                (PROPERTY::TITLE_P, TITLE)
                (PROPERTY::TITLE_ID_P, TITLE_ID)
                (PROPERTY::TITLE_TYPE_P, TITLE_TYPE)
                (PROPERTY::GENRE_P, GENRE)
                (PROPERTY::START_YEAR_P, START_YEAR)
                (PROPERTY::END_YEAR_P, END_YEAR)
                (PROPERTY::DIRECTORS_P, DIRECTORS)
                (PROPERTY::WRITERS_P, WRITERS)
                (PROPERTY::ACTORS_P, ACTORS)
                (PROPERTY::RUNTIME_P, RUNTIME_MINUTES)
                (PROPERTY::REGION_P, REGION)
                (PROPERTY::LANGUAGE_P, LANGUAGE)
                (PROPERTY::PARENT_CONST_P, PARENT_CONST)
                (PROPERTY::EPISODE_P, EPISODE)
                (PROPERTY::SEASON_P, SEASON)
                (PROPERTY::PATH_P, PATH)
                (PROPERTY::LIBRARY_ID_P, LIBRARY_ID)
                (PROPERTY::FILENAME_P, FILENAME)
                ;
    }

    void TearDown() override
    {
        //
    }
};


TEST_F(CacheTest, json1) {
    Database obj;
    obj.m_title = "Test";
    obj.m_titleId = "tt11223344";
    obj.m_writers = "dfsdf";
    obj.m_directors = "fdsfd";
    obj.m_actors = "fsdfsdf";
    obj.m_library_id = "1";
    obj.m_startYear = "1989";
    obj.m_runtimeMinutes = "132";
    obj.m_genre = "romance";
    obj.m_language = "SV";
    obj.m_filename = "fname";
    JsonStreamer<Database, PROPERTY> streamer("db_cache.json", _propertyMap);
    streamer.update(obj);
}


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

TEST_F(CacheTest, test3) {

    std::string testFile = "Enemy.Of.the.State.1998.1080p.BrRip.x264.BOKUTOX.YIFY.mp4";
    CacheMgr::getInstance().validate(testFile);
    auto db = CacheMgr::getInstance().get(testFile);
    ASSERT_TRUE(db.m_title.find("Enemy of the State") != std::string::npos);
    ASSERT_TRUE(db.m_titleId.find("tt0120660") != std::string::npos);
    ASSERT_TRUE(db.m_genre.find("Action") != std::string::npos);
    ASSERT_TRUE(db.m_directors.find("Tony Scott") != std::string::npos);
    ASSERT_FALSE(db.m_runtimeMinutes.find("102") != std::string::npos);
    ASSERT_FALSE(db.m_titleType.find("series") != std::string::npos);
}


#endif //SYNOFW_CACHETEST_H
