//
// Created by mjonsson on 5/2/19.
//

#ifndef SYNOFW_CACHEMGR_H
#define SYNOFW_CACHEMGR_H

#include "Logger.h"
#include "Imdb.h"
#include "Subject.h"
#include "Utilities.h"
#include "ImdbAkas.h"
#include "ImdbBasics.h"
#include "ImdbCrew.h"
#include "ImdbName.h"
#include "JsonStreamer.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <functional>
#include <map>
#include <assert.h>
#include <mutex>
#include <chrono>

class Logger;


class CacheMgr : Subject{
private:
    CacheMgr() {
        streamer.setFile("db_cache.json");
    };
    ~CacheMgr() = default;
    JsonStreamer streamer;

    bool update(std::string&);

    struct TitleCommonS {
        const unsigned short titleId = 0;
    }commonS;

    struct TitleAkasS {
        const unsigned short ordering = 1;
        const unsigned short title = 2;
        const unsigned short region = 3;
        const unsigned short language = 4;
    }akasS;

    struct TitleBasicsS {
        const unsigned short startYear = 2;
        const unsigned short endYear = 3;
        const unsigned short genre = 5;
    }basicsS;

    struct TitleCrewS {
        const unsigned short directors = 1; //array of string
        const unsigned short writers = 2; //array of string
    }crewS;

    struct TitleEpisodeS {
        const unsigned short season = 2; //int
        const unsigned short episode = 3; //int
    }episodeS;

    struct NamesS {
        const unsigned short nconst = 0;
        const unsigned short primaryName = 1;
    }nameS;


public:
    static CacheMgr& getInstance();
    CacheMgr(CacheMgr const&) = delete;
    void operator=(CacheMgr const&) = delete;

    void validate(std::string&);
    DatabaseObject get(std::string&);
};



#endif //SYNOFW_IMDBSTRUCTURE_H
