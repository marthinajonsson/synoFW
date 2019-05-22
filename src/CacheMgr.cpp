
#include <CacheMgr.h>

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
    auto start = std::chrono::high_resolution_clock::now();
    ImdbAkas a;
    std::map<std::string, std::string> result = a.parse({akasS.title, title}, {{commonS.titleId, ""}});
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;
    assert(!result.empty());
    DatabaseObject obj;
    obj.m_titleId = result.at("titleId");

    start = std::chrono::high_resolution_clock::now();
    result = a.parse({akasS.title, title}, {{akasS.region, ""}});
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;
    assert(!result.empty());
    obj.m_region = result.at("region");

    start = std::chrono::high_resolution_clock::now();
    result = a.parse({akasS.title, title}, {{akasS.language, ""}});
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;
    assert(!result.empty());
    obj.m_language = result.at("language");

    ImdbBasics b;
    start = std::chrono::high_resolution_clock::now();
    result = b.parse({commonS.titleId, obj.m_titleId}, {{basicsS.genre, ""}});
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;

    assert(!result.empty());
    obj.m_genre = result.at("genre");

    start = std::chrono::high_resolution_clock::now();
    result = b.parse({commonS.titleId, obj.m_titleId}, {{basicsS.startYear, ""}});
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;

    assert(!result.empty());
    obj.m_startYear = result.at("startYear");

    start = std::chrono::high_resolution_clock::now();
    result = b.parse({commonS.titleId, obj.m_titleId}, {{basicsS.endYear, ""}});
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;

    assert(!result.empty());
    obj.m_endYear = result.at("endYear");

//        start = std::chrono::high_resolution_clock::now();
//        result2 = parse({common.titleId, id}, {{crew.directors, ""}, {crew.writers, ""}});
//        stop = std::chrono::high_resolution_clock::now();
//        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
//        std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;
//        std::string nameIds = result2.at(getMetaMapping("title.crew.tsv").at(crew.directors));
//        auto directors = split(nameIds, ',');
//
//        std::string namesOfCrew;
//        for (auto &d : directors) {
//            auto tmp = parse("name.basics.tsv", {name.nconst, d}, {{name.primaryName, ""}});
//            auto map = getMetaMapping("name.basics.tsv");
//            std::string primaryName = tmp.at(map.at(name.primaryName));
//            namesOfCrew.append(primaryName);
//            namesOfCrew.append(", ");
//        }

//        namesOfCrew.erase (namesOfCrew.end()-2);
//        result.insert(std::make_pair("directors", namesOfCrew));
//
//        nameIds = result2.at(getMetaMapping("title.crew.tsv").at(crew.writers));
//        auto writers = split(nameIds, ',');
//
//        namesOfCrew.clear();
//        for (auto &w : writers) {
//            result2 = parse("name.basics.tsv", {name.nconst, w}, {{name.primaryName, ""}});
//            auto map = getMetaMapping("name.basics.tsv");
//            std::string primaryName = result2.at(map.at(name.primaryName));
//            namesOfCrew.append(primaryName);
//            namesOfCrew.append(", ");
//        }
//        namesOfCrew.erase (namesOfCrew.end()-2);
//        result.insert(std::make_pair("writers", namesOfCrew));

    streamer.update(obj);
    return true;
}


