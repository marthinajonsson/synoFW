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

    struct TitleAkas {
        const unsigned char titleId = 0;
        const unsigned char ordering = 1;
        const unsigned char title = 2;
        const unsigned char region = 3;
        const unsigned char language = 4;
    }akas;

    struct TitleBasics {
        const unsigned char titleId = 0;
        const unsigned char primaryTitle = 2;
        const unsigned char originalTitle = 3;
        const unsigned char startYear = 5;
        const unsigned char endYear = 6;
        const unsigned char genre = 8;
    }basics;

    std::vector<std::string> parse(std::string &&filename, std::string &&findStr, const short &column = 99, const short& compareColumn = 99, std::string&& compareStr = "");

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

        auto resultIdn = parse("title.akas.tsv", "Woman in Gold", akas.titleId, akas.titleId, "tt2404425");
        for(auto &s : resultIdn) {
            std::cout << s << std::endl;
        }
    }

};



#endif //SYNOFW_IMDBSTRUCTURE_H
