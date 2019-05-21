//
// Created by mjonsson on 5/2/19.
//

#ifndef SYNOFW_IMDBSTRUCTURE_H
#define SYNOFW_IMDBSTRUCTURE_H

#include "Logger.h"
#include "Imdb.h"
#include "Subject.h"
#include "Utilities.h"
#include "ImdbAkas.h"
#include "ImdbBasics.h"

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
class ImdbStructure;

std::mutex singleImdb;
static ImdbStructure *instance;

class ImdbStructure : Subject{
private:
    ImdbStructure() = default;
    ~ImdbStructure() = default;

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
        const unsigned short startYear = 5;
        const unsigned short endYear = 6;
        const unsigned short genre = 8;
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

    static ImdbStructure& getInstance()
    {
        std::lock_guard<std::mutex> lock(singleImdb);
        if(!instance) {
            instance = new ImdbStructure();
        }
        return *instance;
    }
    ImdbStructure(ImdbStructure const&) = delete;
    void operator=(ImdbStructure const&) = delete;



    bool parseTitle(std::shared_ptr<Logger> &pLog, std::string& title)
    {
        assert(!title.empty());
        auto start = std::chrono::high_resolution_clock::now();
        ImdbAkas akas("title.akas.tsv");
        std::map<std::string, std::string> result = akas.parse({akasS.title, title}, {{commonS.titleId, ""}});
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        std::map<std::string, std::string> result2 = akas.parse({akasS.title, title}, {{akasS.language, ""}});
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        std::map<std::string, std::string> result3 = akas.parse({akasS.title, title}, {{akasS.region, ""}});
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;

        ImdbBasics basics("title.basics.tsv");
        start = std::chrono::high_resolution_clock::now();
        std::map<std::string, std::string> result4 = basics.parse({commonS.titleId, "ImdbId...."}, {{basicsS.genre, ""}});
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        std::map<std::string, std::string> result5 = basics.parse({commonS.titleId, "ImdbId...."}, {{basicsS.startYear, ""}});
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;


        start = std::chrono::high_resolution_clock::now();
        std::map<std::string, std::string> result6 = basics.parse({commonS.titleId, "ImdbId...."}, {{basicsS.endYear, ""}});
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Exec time " << duration.count() << " [ms]" << std::endl;
//
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
//
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

        result.insert(result2.begin(), result2.end());
        result.insert(result3.begin(), result3.end());

        result.insert(result4.begin(), result4.end());
        result.insert(result5.begin(), result5.end());
        result.insert(result6.begin(), result6.end());

        for(auto &s : result) {
            //std::cout << s.first << ":" << s.second << std::endl;
            pLog->writeLog(SeverityType::GENERAL, s.first + ":" + s.second);
        }
        //pLog->writeLog(SeverityType::ERROR, "Parsing completed with error");
        return true;
    }

};



#endif //SYNOFW_IMDBSTRUCTURE_H
