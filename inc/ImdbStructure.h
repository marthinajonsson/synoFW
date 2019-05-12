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

    std::map<unsigned short, std::string> metadataMapper;

    ImdbStructure() {
        metadataMapper.insert ( std::pair<unsigned short, std::string>(common.titleId,"titleId") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(akas.title,"title") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(basics.startYear, "startYear") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(basics.endYear, "endYear") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(basics.genre, "genre") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(crew.directors, "directors") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(crew.writers, "writers") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(episode.season, "season") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(episode.episode, "episode") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(name.nconst, "nconst") );
        metadataMapper.insert ( std::pair<unsigned short, std::string>(name.primaryName, "primaryName") );
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
        unpackFile("title.akas.tsv.gz");
        unpackFile("title.basics.tsv.gz");
        unpackFile("title.crew.tsv.gz");
        unpackFile("title.episode.tsv.gz");
        unpackFile("name.basics.tsv.gz");
        pLog->writeLog(SeverityType::GENERAL, "IMDB files are decompressed");
    }

    void parseTitle(std::shared_ptr<Logger> &pLog, std::string& title)
    {
        assert(!title.empty());
        pLog->writeLog(SeverityType::GENERAL, "Parsing " + title + ".. ");
        std::map<std::string, std::string> result = parse("title.akas.tsv", {akas.title, title}, {{common.titleId, ""}});
        std::string id = result.at(metadataMapper.at(common.titleId));

        pLog->writeLog(SeverityType::GENERAL, "Found IMDB id " + id);
        std::map<std::string, std::string> result2 = parse("title.basics.tsv", {common.titleId, id}, {{basics.genre, ""}, {basics.startYear, ""}, {basics.endYear, ""}});

        result.insert(result2.begin(), result2.end());
        result2 = parse("title.crew.tsv", {common.titleId, id}, {{crew.directors, ""}, {crew.writers, ""}});

        result.insert(result2.begin(), result2.end());

        std::string nameId = result.at(metadataMapper.at(name.nconst));
        result2 = parse("name.basics.tsv", {name.nconst, nameId}, {{name.primaryName, ""}});
        result.insert(result2.begin(), result2.end());

        pLog->writeLog(SeverityType::GENERAL, "Found IMDB crew name ..");

        for(auto &s : result) {
            std::cout << s.first << ":" << s.second << std::endl;
        }
        pLog->writeLog(SeverityType::ERROR, "Parsing completed with error");
    }

};



#endif //SYNOFW_IMDBSTRUCTURE_H
