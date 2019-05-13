//
// Created by mjonsson on 5/2/19.
//

#ifndef SYNOFW_IMDBSTRUCTURE_H
#define SYNOFW_IMDBSTRUCTURE_H

#include "Logger.h"
#include "Subject.h"
#include "Utilities.h"

#include <curl/curl.h>
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

class Logger;

class ImdbStructure : Subject{
private:
    TitleCommon common;
    TitleAkas akas;
    TitleBasics basics;
    TitleCrew crew;
    TitleEpisode episode;
    Names name;

    std::map<std::string, std::map<unsigned short, std::string>> metaMapper;

    ImdbStructure() {
        std::map<unsigned short, std::string> metaMap;
        metaMap.insert ( std::pair<unsigned short, std::string>(common.titleId,"titleId") );
        metaMap.insert ( std::pair<unsigned short, std::string>(akas.title,"title") );
        metaMapper.insert(std::make_pair("title.akas.tsv", metaMap));
        metaMap.clear();

        metaMap.insert ( std::pair<unsigned short, std::string>(common.titleId,"titleId") );
        metaMap.insert ( std::pair<unsigned short, std::string>(basics.startYear, "startYear") );
        metaMap.insert ( std::pair<unsigned short, std::string>(basics.endYear, "endYear") );
        metaMap.insert ( std::pair<unsigned short, std::string>(basics.genre, "genre") );
        metaMapper.insert(std::make_pair("title.basics.tsv", metaMap));
        metaMap.clear();

        metaMap.insert ( std::pair<unsigned short, std::string>(common.titleId,"titleId") );
        metaMap.insert ( std::pair<unsigned short, std::string>(crew.directors, "directors") );
        metaMap.insert ( std::pair<unsigned short, std::string>(crew.writers, "writers") );
        metaMapper.insert(std::make_pair("title.crew.tsv", metaMap));
        metaMap.clear();

        metaMap.insert ( std::pair<unsigned short, std::string>(episode.season, "season") );
        metaMap.insert ( std::pair<unsigned short, std::string>(episode.episode, "episode") );
        metaMapper.insert(std::make_pair("title.episode.tsv", metaMap));
        metaMap.clear();

        metaMap.insert ( std::pair<unsigned short, std::string>(name.nconst, "nconst") );
        metaMap.insert ( std::pair<unsigned short, std::string>(name.primaryName, "primaryName") );
        metaMapper.insert(std::make_pair("name.basics.tsv", metaMap));
        metaMap.clear();
    }

    const std::map<unsigned short, std::string>& getMetaMapping(const std::string& filename) {
        return metaMapper.at(filename);
    }

    std::string unpackFile(std::string&& file);
    std::string getDataFiles(std::string&& file);
    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
    std::map<std::string, std::string> parse(const std::string &&, std::pair<unsigned short, std::string> && = {}, std::vector<std::pair<unsigned short, std::string>> && = {});

public:

    static ImdbStructure& getInstance();
    ImdbStructure(ImdbStructure const&) = delete;
    void operator=(ImdbStructure const&) = delete;

    void fetch(std::shared_ptr<Logger> &pLog) {
        pLog->writeLog(SeverityType::GENERAL, "Downloading compressed files.. ");
        auto files = {"title.akas.tsv.gz", "title.basics.tsv.gz", "title.crew.tsv.gz", "title.episode.tsv.gz", "name.basics.tsv.gz"};

        for(auto f : files) {
            std::string err = getDataFiles(f);
            if(!err.empty()) {
                std::string info = "Compressed file ";
                info.append(f);
                info.append(" failed to download due to ");
                info.append(err);
                pLog->writeLog(SeverityType::WARNING, info);
            }
        }
        pLog->writeLog(SeverityType::GENERAL, "Downloading complete");


        pLog->writeLog(SeverityType::GENERAL, "Decompress files.. ");
        for(auto f : files) {
            std::string err = unpackFile(f);
            if(!err.empty()) {
                std::string info = "Decompressing file ";
                info.append(f);
                info.append(" failed due to ");
                info.append(err);
                pLog->writeLog(SeverityType::WARNING, info);
            }
        }
        pLog->writeLog(SeverityType::GENERAL, "IMDB files are decompressed");
    }

    bool parseTitle(std::shared_ptr<Logger> &pLog, std::string& title)
    {
        std::cout.flush();
        assert(!title.empty());

        std::map<std::string, std::string> result = parse("title.akas.tsv", {akas.title, title}, {{common.titleId, ""}});
        std::string id = result.at(getMetaMapping("title.akas.tsv").at(common.titleId));

        std::map<std::string, std::string> result2 = parse("title.basics.tsv", {common.titleId, id}, {{basics.genre, ""}, {basics.startYear, ""}, {basics.endYear, ""}});
        result.insert(result2.begin(), result2.end());


        result2 = parse("title.crew.tsv", {common.titleId, id}, {{crew.directors, ""}, {crew.writers, ""}});
        std::string nameIds = result2.at(getMetaMapping("title.crew.tsv").at(crew.directors));
        auto directors = split(nameIds, ',');

        std::string namesOfCrew;
        for (auto &d : directors) {
            auto tmp = parse("name.basics.tsv", {name.nconst, d}, {{name.primaryName, ""}});
            auto map = getMetaMapping("name.basics.tsv");
            std::string primaryName = tmp.at(map.at(name.primaryName));
            namesOfCrew.append(primaryName);
            namesOfCrew.append(", ");
        }

        namesOfCrew.erase (namesOfCrew.end()-2);
        result.insert(std::make_pair("directors", namesOfCrew));

        nameIds = result2.at(getMetaMapping("title.crew.tsv").at(crew.writers));
        auto writers = split(nameIds, ',');

        namesOfCrew.clear();
        for (auto &w : writers) {
            result2 = parse("name.basics.tsv", {name.nconst, w}, {{name.primaryName, ""}});
            auto map = getMetaMapping("name.basics.tsv");
            std::string primaryName = result2.at(map.at(name.primaryName));
            namesOfCrew.append(primaryName);
            namesOfCrew.append(", ");
        }
        namesOfCrew.erase (namesOfCrew.end()-2);
        result.insert(std::make_pair("writers", namesOfCrew));

        for(auto &s : result) {
            //std::cout << s.first << ":" << s.second << std::endl;
            pLog->writeLog(SeverityType::GENERAL, s.first + ":" + s.second);
        }
        pLog->writeLog(SeverityType::ERROR, "Parsing completed with error");
        return true;
    }

};



#endif //SYNOFW_IMDBSTRUCTURE_H
