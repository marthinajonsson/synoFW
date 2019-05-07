//
// Created by mjonsson on 5/2/19.
//

#ifndef SYNOFW_IMDBSTRUCTURE_H
#define SYNOFW_IMDBSTRUCTURE_H

#include <curl/curl.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>
#include <assert.h>


class ImdbStructure {
private:

    struct TitleCommon {
        const unsigned short titleId = 0;
    }common;

    struct TitleAkas {
        const unsigned short ordering = 1;
        const unsigned short title = 2;
        const unsigned short region = 3;
        const unsigned short language = 4;
    }akas;

    struct TitleBasics {
        const unsigned short primaryTitle = 2;
        const unsigned short originalTitle = 3;
        const unsigned short startYear = 5;
        const unsigned short endYear = 6;
        const unsigned short genre = 8;
    }basics;

    struct TitleCrew {
        const unsigned short directors = 1; //array of string
        const unsigned short writers = 2; //array of string
    }crew;

    struct TitleEpisode {
        const unsigned short season = 2; //int
        const unsigned short episode = 3; //int
    }episode;
    
    std::map<unsigned short, std::string> metadataMapper;
    std::map<std::string, std::string> parse(const std::string &&, std::pair<unsigned short, std::string> && = {}, std::vector<std::pair<unsigned short, std::string>> && = {});

    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
    void unpackFile(std::string&& file);
    void getDataFiles(std::string&& file);

public:

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
    }

    void FetchDatabase(std::string& title)
    {

//        getDataFiles("title.akas.tsv.gz");
//        getDataFiles("title.basics.tsv.gz");
//        getDataFiles("title.crew.tsv.gz");
//        getDataFiles("title.episode.tsv.gz");
//
//        unpackFile("title.akas.tsv.gz");
//        unpackFile("title.basics.tsv.gz");
//        unpackFile("title.crew.tsv.gz");
//        unpackFile("title.episode.tsv.gz");

        assert(!title.empty());
        std::map<std::string, std::string> result = parse("title.akas.tsv", {akas.title, title}, {{common.titleId, ""}});
        std::string id = result.at(metadataMapper.at(common.titleId));

        std::map<std::string, std::string> result2 = parse("title.basics.tsv", {common.titleId, id}, {{basics.genre, ""}, {basics.startYear, ""}, {basics.endYear, ""}});

        result.insert(result2.begin(), result2.end());
        result2 = parse("title.crew.tsv", {common.titleId, id}, {{crew.directors, ""}, {crew.writers, ""}});

        result.insert(result2.begin(), result2.end());


        /*
         *  Directors nmxxxxxx used with name.basics.tsv.gz
         *  Contains the following information for names: nconst (string) - alphanumeric unique identifier of the name/person
         * */
        for(auto &s : result) {
            std::cout << s.first << ":" << s.second << std::endl;
        }
    }

};



#endif //SYNOFW_IMDBSTRUCTURE_H
