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


class ImdbStructure {
private:

    enum class AkasE {
        titleId = 0,
        ordering = 1,
        title = 2,
        region = 3,
        language = 4
    };

    enum class BasicsE {
        titleId = 0,
        OriginalTitle = 3,
        startYear = 5,
        endYear = 6,
        genre = 8
    };

    struct TitleAkas {
        const unsigned short titleId = 0;
        const unsigned short ordering = 1;
        const unsigned short title = 2;
        const unsigned short region = 3;
        const unsigned short language = 4;
    }akas;

    struct TitleBasics {
        const unsigned short titleId = 0;
        const unsigned short primaryTitle = 2;
        const unsigned short originalTitle = 3;
        const unsigned short startYear = 5;
        const unsigned short endYear = 6;
        const unsigned short genre = 8;
    }basics;

    std::vector<std::string> parse(const std::string &&, const std::string &&, std::vector<std::pair<unsigned short, std::string>> && = {});
    // filter column {column:value, column:value}

    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
    void unpackFile(std::string&& file);
    void getDataFiles(std::string&& file);

public:

    void FetchDatabase() {

//        getDataFiles("title.akas.tsv.gz");
//        getDataFiles("title.basics.tsv.gz");
//        getDataFiles("title.crew.tsv.gz");
//        getDataFiles("title.episode.tsv.gz");

//        unpackFile("title.akas.tsv.gz");
//        unpackFile("title.basics.tsv.gz");

        //tt2404425
//        std::vector<std::string> resultIdn = parse("title.akas.tsv", "Woman in Gold", akas.titleId);
//        for(auto &s : resultIdn) {
//            std::cout << s << std::endl;
//        }
//
//        resultIdn = parse("title.akas.tsv", "Woman in Gold");
//        for(auto &s : resultIdn) {
//            std::cout << s << std::endl;
//        }

        auto resultIdn = parse("title.akas.tsv", "Woman in Gold", {{akas.titleId, "tt2404425"}, {akas.title, "Woman in Gold"}});
        for(auto &s : resultIdn) {
            std::cout << s << std::endl;
        }
    }

};



#endif //SYNOFW_IMDBSTRUCTURE_H
