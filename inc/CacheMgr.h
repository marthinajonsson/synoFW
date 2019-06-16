//
// Created by mjonsson on 5/2/19.
//

#ifndef SYNOFW_CACHEMGR_H
#define SYNOFW_CACHEMGR_H

#include "JsonStreamer.h"
#include "EventLogger.h"
#include "Logger.h"

#include <string>
#include <memory>

class CacheMgr{
public:
    static CacheMgr& getInstance();
    CacheMgr(CacheMgr const&) = delete;
    void operator=(CacheMgr const&) = delete;

    void validate(std::string&);
    struct database get(std::string&);
    void edit (database&);
private:
    std::unique_ptr<Logger> log_ptr = std::make_unique<Logger>();
    EventLogger* elog_ptr;

    CacheMgr () {
        elog_ptr = new EventLogger();
        log_ptr->registerObserver(SeverityType::GENERAL, elog_ptr);
    };

    ~CacheMgr () {
        log_ptr->removeObserver(elog_ptr);
        delete elog_ptr;
    };
    std::unique_ptr<JsonStreamer> stream_ptr = std::make_unique<JsonStreamer>("db_cache.json");

    struct TitleAkasS {
        const unsigned short titleId = 0;
        const unsigned short ordering = 1;
        const unsigned short title = 2;
        const unsigned short region = 3;
        const unsigned short language = 4;
    }akasS;

    struct TitleBasicsS {
        const unsigned short titleId = 0;
        const unsigned short titletype = 1;
        const unsigned short startYear = 5;
        const unsigned short endYear = 6;
        const unsigned short runtimeMinutes = 7;
        const unsigned short genre = 8;
    }basicsS;

    struct TitleCrewS {
        const unsigned short titleId = 0; // id of title
        const unsigned short nconst = 2; //id of person
        const unsigned short category = 3; //role of person
    }crewS;

    struct TitleEpisodeS {
        const unsigned short epsiodeId = 0;
        const unsigned short parentTconst = 1;
        const unsigned short season = 2; //int
        const unsigned short episode = 3; //int
    }episodeS;

    struct NamesS {
        const unsigned short nconst = 0;
        const unsigned short primaryName = 1;
    }nameS;


    bool update(std::string&, std::string&);
};



#endif //SYNOFW_IMDBSTRUCTURE_H
