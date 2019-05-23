#include "CacheMgr.h"
#include "JsonStreamer.h"
#include "FilenameStructure.h"


std::mutex singleCache;
static CacheMgr *instance;

CacheMgr& CacheMgr::getInstance()
{
    std::lock_guard<std::mutex> lock(singleCache);
    if(!instance) {
        instance = new CacheMgr();
    }
    return *instance;
}

DatabaseObject CacheMgr::get(std::string& result) {
    FilenameStructure fn;
    fn.parse(std::move(result));
    auto title = fn.getTitle();
    return streamer.find(title);
}

void CacheMgr::validate(std::string& result) {
    FilenameStructure fn;
    fn.parse(std::move(result));
    auto title = fn.getTitle();
    if(streamer.checkForNull(title)) {
        this->update(title);
    }
}

bool CacheMgr::update(std::string& title)
{
    assert(!title.empty());

    DatabaseObject obj;

    ImdbAkas a;
    ImdbBasics b;
    ImdbCrew c;
    ImdbEpisode e;
    ImdbName n;

    std::map<std::string, std::string> result = a.parse({akasS.title, title}, {{akasS.titleId, ""}});
    assert(result.size() > 1);

    obj.m_title = result.at("title");
    obj.m_titleId = result.at("titleId");

    result = a.parse({akasS.title, title}, {{akasS.language, ""}});
    assert(result.size() > 1);
    obj.m_language = result.at("language");

    result = a.parse({akasS.title, title}, {{akasS.region, ""}});
    assert(result.size() > 1);
    obj.m_region = result.at("region");

    result = b.parse({basicsS.titleId, obj.m_titleId}, {{basicsS.titletype, ""}});
    assert(result.size() > 1);
    obj.m_titleType = result.at("titleType");

    if(obj.m_titleType == "series") {
        result = b.parse({basicsS.titleId, obj.m_titleId}, {{basicsS.endYear, ""}});
        assert(result.size() > 1);
        obj.m_endYear = result.at("endYear");

        result = e.parse({episodeS.parentTconst, obj.m_titleId}, {{episodeS.season, ""}, {episodeS.episode, ""}});
        assert(result.size() > 1);
        obj.m_season = result.at("season");
        obj.m_episode = result.at("episode");
    }

    result = b.parse({basicsS.titleId, obj.m_titleId}, {{basicsS.startYear, ""}});
    assert(result.size() > 1);
    obj.m_startYear = result.at("startYear");
    result = b.parse({basicsS.titleId, obj.m_titleId}, {{basicsS.runtimeMinutes, ""}});
    assert(result.size() > 1);
    obj.m_runtimeMinutes = result.at("runtimeMinutes");

    result = b.parse({basicsS.titleId, obj.m_titleId}, {{basicsS.genre, ""}});
    assert(!result.empty());
    obj.m_genre = result.at("genre");

    result = c.parse({crewS.titleId, obj.m_titleId}, {{crewS.directors, ""}, {crewS.writers, ""}});

    std::string nameIds = result.at("directors");
    auto directors = split(nameIds, ',');
    nameIds = result.at("writers");
    auto writers = split(nameIds, ',');

    std::string namesOfCrew;
    for (auto &d : directors) {
        auto tmp = n.parse({nameS.nconst, d}, {{nameS.primaryName, ""}});
        if(tmp.size() < 2) {
            break;
        }
        std::string primaryName = tmp.at("primaryName");
        namesOfCrew.append(primaryName);
        namesOfCrew.append(", ");
    }
    namesOfCrew.erase (namesOfCrew.end()-2);
    obj.m_directors = namesOfCrew;
    namesOfCrew = "";

    for (auto &w : writers) {
        auto tmp = n.parse({nameS.nconst, w}, {{nameS.primaryName, ""}});
        if(tmp.size() < 2) {
            break;
        }
        std::string primaryName = tmp.at("primaryName");
        namesOfCrew.append(primaryName);
        namesOfCrew.append(", ");
    }
    namesOfCrew.erase (namesOfCrew.end()-2);
    obj.m_writers = namesOfCrew;

    streamer.update(obj);
    return true;
}


