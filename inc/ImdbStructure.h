//
// Created by mjonsson on 5/2/19.
//

#ifndef SYNOFW_IMDBSTRUCTURE_H
#define SYNOFW_IMDBSTRUCTURE_H

#include <Subject.h>
#include <Utilities.h>
#include "StatusLogger.h"

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

using namespace Pattern;

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


    void unpackFile(std::string&& file);
    void getDataFiles(std::string&& file);
    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
    std::map<std::string, std::string> parse(const std::string &&, std::pair<unsigned short, std::string> && = {}, std::vector<std::pair<unsigned short, std::string>> && = {});

public:

    static ImdbStructure& getInstance();
    ImdbStructure(ImdbStructure const&) = delete;
    void operator=(ImdbStructure const&) = delete;

    void fetch() {
        //notifyObservers(Status::DownloadOngoing);

        getDataFiles("title.akas.tsv.gz");
        getDataFiles("title.basics.tsv.gz");
        getDataFiles("title.crew.tsv.gz");
        getDataFiles("title.episode.tsv.gz");
        getDataFiles("name.basics.tsv.gz");

        unpackFile("title.akas.tsv.gz");
        unpackFile("title.basics.tsv.gz");
        unpackFile("title.crew.tsv.gz");
        unpackFile("title.episode.tsv.gz");
        unpackFile("name.basics.tsv.gz");

    //    notifyObservers(Status::DownloadCompleted);

    }

    void parseTitle(std::string& title)
    {
        assert(!title.empty());
     //   notifyObservers(Status::ParsingOngoing);
        std::map<std::string, std::string> result = parse("title.akas.tsv", {akas.title, title}, {{common.titleId, ""}});
        std::string id = result.at(metadataMapper.at(common.titleId));

        std::map<std::string, std::string> result2 = parse("title.basics.tsv", {common.titleId, id}, {{basics.genre, ""}, {basics.startYear, ""}, {basics.endYear, ""}});

        result.insert(result2.begin(), result2.end());
        result2 = parse("title.crew.tsv", {common.titleId, id}, {{crew.directors, ""}, {crew.writers, ""}});

        result.insert(result2.begin(), result2.end());


        std::string nameId = result.at(metadataMapper.at(name.nconst));
        result2 = parse("name.basics.tsv", {name.nconst, nameId}, {{name.primaryName, ""}});
        result.insert(result2.begin(), result2.end());

        for(auto &s : result) {
            std::cout << s.first << ":" << s.second << std::endl;
        }
     //   notifyObservers(Status::ParsingCompleted);
    }

};



#endif //SYNOFW_IMDBSTRUCTURE_H
