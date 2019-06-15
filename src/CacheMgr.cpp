#include "FilenameStructure.h"
#include "CacheMgr.h"
#include "FileMgr.h"
#include "Logger.h"
#include "Imdb.h"

#include <boost/assert.hpp>
#include <vector>
#include <map>

std::mutex singleCache;
static CacheMgr *instance;

CacheMgr& CacheMgr::getInstance ()
{
    std::lock_guard<std::mutex> lock(singleCache);
    if(!instance) {
        instance = new CacheMgr();
    }
    return *instance;
}

void CacheMgr::edit (database obj) {
    streamer.update(obj);
}

struct database CacheMgr::get (std::string& result) {
    FilenameStructure fn;
    fn.parse(std::move(result));
    auto title = fn.getTitle();
    return streamer.find(title);
}

void CacheMgr::validate (std::string& result) {
    FilenameStructure fn;
    fn.parse(std::move(result));
    auto title = fn.getTitle();
    auto year = fn.getYear();
    if(streamer.checkForNull(title)) {
        this->update(title, year);
    }
}

bool CacheMgr::update (std::string& title, std::string& year)
{
    BOOST_ASSERT(!title.empty());

    database obj;

    Imdb imdb;
    imdb.loadfile("title.akas.tsv");

    obj.m_title = title;

    auto mMap = imdb.parse <std::multimap<std::string, std::string>> ({akasS.title, title}, {{akasS.titleId, ""}});
    BOOST_ASSERT(!mMap.empty());
    auto equals = mMap.equal_range("titleId");

    std::multimap<std::string,std::string>::const_iterator it;
    for (it = equals.first; it != equals.second; it++) {
        obj.m_titleId = it->second;
        imdb.loadfile("title.basics.tsv");
        auto uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, it->second}, {{basicsS.startYear, ""}});
        BOOST_ASSERT(uniqueMap.count("startYear") == 1);
        obj.m_startYear = uniqueMap.at("startYear");
        if (obj.m_startYear.find(year) != std::string::npos)
            break;
    }

    imdb.loadfile("title.basics.tsv");
    auto uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, obj.m_titleId}, {{basicsS.titletype, ""}});
    BOOST_ASSERT(!uniqueMap.empty());
    obj.m_titleType = uniqueMap.at("titleType");

    if(obj.m_titleType == "series") {
        uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, obj.m_titleId}, {{basicsS.endYear, ""}});
        BOOST_ASSERT(!uniqueMap.empty());
        obj.m_endYear = uniqueMap.at("endYear");

        imdb.loadfile("title.episode.tsv");
        uniqueMap = imdb.parse <std::map<std::string, std::string>> ({episodeS.parentTconst, obj.m_titleId}, {{episodeS.season, ""}, {episodeS.episode, ""}});
        BOOST_ASSERT(!uniqueMap.empty());
        obj.m_season = uniqueMap.at("season");
        obj.m_episode = uniqueMap.at("episode");
    }

    imdb.loadfile("title.basics.tsv");
    uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, obj.m_titleId}, {{basicsS.runtimeMinutes, ""}});
    BOOST_ASSERT(!uniqueMap.empty());
    obj.m_runtimeMinutes = uniqueMap.at("runtimeMinutes");

    uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, obj.m_titleId}, {{basicsS.genre, ""}});
    BOOST_ASSERT(!uniqueMap.empty());
    obj.m_genre = uniqueMap.at("genre");

    imdb.loadfile("title.crew.tsv");
    uniqueMap = imdb.parse <std::map<std::string, std::string>> ({crewS.titleId, obj.m_titleId}, {{crewS.directors, ""}, {crewS.writers, ""}});
    BOOST_ASSERT(!uniqueMap.empty());

    std::string nameIds = uniqueMap.at("directors");
    auto directors = split(nameIds, ',');
    nameIds = uniqueMap.at("writers");
    auto writers = split(nameIds, ',');

    imdb.loadfile("name.basics.tsv");
    std::string namesOfCrew;
    for (auto &d : directors) {
        auto tmp = imdb.parse <std::map<std::string, std::string>> ({nameS.nconst, d}, {{nameS.primaryName, ""}});
        if (tmp.find("primaryName") != tmp.end()) {
            std::string primaryName = tmp.at("primaryName");
            namesOfCrew.append(primaryName);
            namesOfCrew.append(", ");
        }
    }
    if(namesOfCrew.size() > 2) {
        namesOfCrew.erase (namesOfCrew.end()-2);
    }
    obj.m_directors = !namesOfCrew.empty() ? namesOfCrew : "N";
    namesOfCrew = "";

    for (auto &w : writers) {
        auto tmp = imdb.parse <std::map<std::string, std::string>> ({nameS.nconst, w}, {{nameS.primaryName, ""}});
        if (tmp.find("primaryName") != tmp.end()) {
            std::string primaryName = tmp.at("primaryName");
            namesOfCrew.append(primaryName);
            namesOfCrew.append(", ");
        }
    }
    if(namesOfCrew.size() > 2) {
        namesOfCrew.erase (namesOfCrew.end()-2);
    }
    obj.m_writers = !namesOfCrew.empty() ? namesOfCrew : "N";

    streamer.update(obj);
    return true;
}


