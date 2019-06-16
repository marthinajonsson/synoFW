#include "FilenameStructure.h"
#include "CacheMgr.h"
#include "FileMgr.h"
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

void CacheMgr::edit (database& obj) {
    stream_ptr->update(obj);
}

struct database CacheMgr::get (std::string& result) {
    FilenameStructure fn;
    fn.parse(std::move(result));
    auto title = fn.getTitle();
    return stream_ptr->find(title);
}

void CacheMgr::validate (std::string& result) {
    FilenameStructure fn;
    fn.parse(std::move(result));
    auto title = fn.getTitle();
    auto year = fn.getYear();
    if(stream_ptr->checkForNull(title)) {
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

//    log_ptr->writeLog(SeverityType::GENERAL, "[" +title + "]" + " 0 of 7 update stages completed");
//    auto mMap = imdb.parse <std::multimap<std::string, std::string>> ({akasS.title, title}, {{akasS.titleId, ""}});
//    BOOST_ASSERT(!mMap.empty());
//    auto equals = mMap.equal_range("titleId");
//
//    imdb.loadfile("title.basics.tsv");
//    std::multimap<std::string,std::string>::const_iterator it;
//    for (it = equals.first; it != equals.second; it++) {
//        obj.m_titleId = it->second;
//        auto uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, it->second}, {{basicsS.startYear, ""}});
//        BOOST_ASSERT(uniqueMap.count("startYear") == 1);
//        obj.m_startYear = uniqueMap.at("startYear");
//        if (obj.m_startYear.find(year) != std::string::npos)
//            break;
//    }
//    log_ptr->writeLog(SeverityType::GENERAL, "[" +title + "]" + " 1 of 7 update stages completed");
//
//    auto uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, obj.m_titleId}, {{basicsS.titletype, ""}});
//    BOOST_ASSERT(!uniqueMap.empty());
//    obj.m_titleType = uniqueMap.at("titleType");
//
//    uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, obj.m_titleId}, {{basicsS.runtimeMinutes, ""}});
//    BOOST_ASSERT(!uniqueMap.empty());
//    obj.m_runtimeMinutes = uniqueMap.at("runtimeMinutes");
//
//    uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, obj.m_titleId}, {{basicsS.genre, ""}});
//    BOOST_ASSERT(!uniqueMap.empty());
//    obj.m_genre = uniqueMap.at("genre");
//
//    log_ptr->writeLog(SeverityType::GENERAL, "[" +title + "]" + " 2 of 7 update stages completed");
//
//    if(obj.m_titleType == "series") {
//        uniqueMap = imdb.parse <std::map<std::string, std::string>> ({basicsS.titleId, obj.m_titleId}, {{basicsS.endYear, ""}});
//        BOOST_ASSERT(!uniqueMap.empty());
//        obj.m_endYear = uniqueMap.at("endYear");
//
//        imdb.loadfile("title.episode.tsv");
//        uniqueMap = imdb.parse <std::map<std::string, std::string>> ({episodeS.parentTconst, obj.m_titleId}, {{episodeS.season, ""}, {episodeS.episode, ""}});
//        BOOST_ASSERT(!uniqueMap.empty());
//        obj.m_season = uniqueMap.at("season");
//        obj.m_episode = uniqueMap.at("episode");
//    }
//
//    log_ptr->writeLog(SeverityType::GENERAL, "[" +title + "]" + " 3 of 7 update stages completed");

    obj.m_title = "Woman in Gold";
    obj.m_titleId = "tt2404425";

    std::map<std::string,std::string> testMap;
    auto mapType = typeid(testMap).name();

    imdb.loadfile("title.principals.tsv");
    auto mMap = imdb.parse <std::multimap<std::string, std::string>> ({crewS.titleId, obj.m_titleId}, {{crewS.nconst, ""}, {crewS.category, ""}});
    BOOST_ASSERT(!mMap.empty());
    log_ptr->writeLog(SeverityType::GENERAL, "First");
    auto categoryIt = mMap.equal_range("category"); //actor, producer, writer, actress
    auto nameIdIt = mMap.equal_range("nconst"); // nmxxxxx

    std::multimap<std::string, std::string> crew;
    auto it2 = nameIdIt.first;
    for (auto it = categoryIt.first; it != categoryIt.second && it2 != nameIdIt.second; it++, it2++) {
        auto category = it->second;
        if (category == "producer")
            continue;
        crew.insert(std::make_pair(it->second, it2->second));
    }

    auto directors = crew.equal_range("director");
    auto writer = crew.equal_range("writer");
    auto actor = crew.equal_range("actor");
    auto actress = crew.equal_range("actress");

    mMap.clear();
    for (auto it = actor.first; it != actor.second; it++)
        mMap.insert(std::make_pair(it->first, it->second));
    for (auto it = actress.first; it != actress.second; it++)
        mMap.insert(std::make_pair(it->first, it->second));

    imdb.loadfile("name.basics.tsv");
    std::string namesOfCrew;
    for (auto it = mMap.begin(); it != mMap.end(); it++) {
        auto val = it->second;
        log_ptr->writeLog(SeverityType::GENERAL, "Second Loop");
        auto uniqueMap = imdb.parse <std::map<std::string, std::string>> ({nameS.nconst, it->second}, {{nameS.primaryName, ""}});
        if (uniqueMap.count("primaryName") == 1) {
            auto tmp = uniqueMap.at("primaryName");
            namesOfCrew.append(tmp + ", ");
        }
    }
    if(namesOfCrew.size() > 2) {
        namesOfCrew.erase (namesOfCrew.end()-2);
    }
    obj.m_actors = namesOfCrew;

    log_ptr->writeLog(SeverityType::GENERAL, "[" +title + "]" + " 4 of 7 update stages completed");

    namesOfCrew = "";
    for (auto it = directors.first; it != directors.second; it++) {
        auto val = it->second;
        log_ptr->writeLog(SeverityType::GENERAL, "Third loop");
        auto uniqueMap = imdb.parse <std::map<std::string, std::string>> ({nameS.nconst, it->second}, {{nameS.primaryName, ""}});
        if (uniqueMap.count("primaryName") == 1) {
            auto tmp = uniqueMap.at("primaryName");
            namesOfCrew.append(tmp + ", ");
        }
    }

    if(namesOfCrew.size() > 2) {
        namesOfCrew.erase (namesOfCrew.end()-2);
    }
    obj.m_directors = namesOfCrew;
    log_ptr->writeLog(SeverityType::GENERAL, "[" +title + "]" + " 5 of 7 update stages completed");

    namesOfCrew = "";
    for (auto it = writer.first; it != writer.second; it++) {
        auto val = it->second;
        log_ptr->writeLog(SeverityType::GENERAL, "Fourth loop");
        auto uniqueMap = imdb.parse <std::map<std::string, std::string>> ({nameS.nconst, it->second}, {{nameS.primaryName, ""}});
        if (uniqueMap.count("primaryName") == 1) {
            auto tmp = uniqueMap.at("primaryName");
            namesOfCrew.append(tmp + ", ");
        }
    }

    if(namesOfCrew.size() > 2) {
        namesOfCrew.erase (namesOfCrew.end()-2);
    }
    obj.m_writers = namesOfCrew;


    log_ptr->writeLog(SeverityType::GENERAL, "[" +title + "]" + " 6 of 7 update stages completed");
    stream_ptr->update(obj);
    log_ptr->writeLog(SeverityType::GENERAL, "[" +title + "]" + " 7 of 7 update stages completed");
    return true;
}


