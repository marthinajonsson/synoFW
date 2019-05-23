//
// Created by mjonsson on 5/21/19.
//

#ifndef SYNOFW_IMDBBASICS_H
#define SYNOFW_IMDBBASICS_H

#include <fstream>
#include "Imdb.h"

class Logger;

class ImdbBasics : Imdb {
private:
    std::string imdbFilename = "title.basics.tsv";
    long currentFilePos;
    long headerSize;
    std::map<unsigned short, std::string> mapBasics {
            {0, "titleId"}, {1, "titleType"}, {2, "primaryTitle"}, {3, "originalTitle"}, {5, "startYear"}, {6, "endYear"}, {7, "runtimeMinutes"}, {8, "genre"}
    };


public:
    ImdbBasics() : currentFilePos(0){
        std::fstream file;
        file.open(imdbFilename, std::ios::in);
        currentFilePos = file.tellg();
        std::string tmp;
        getline(file, tmp);
        headerSize = file.tellg();
        file.seekg (0, file.beg);
        file.close();
    }
    ~ImdbBasics() = default;

    std::map<std::string, std::string> parse(std::pair<unsigned short, std::string> &&match, std::vector<std::pair<unsigned short, std::string>> &&find) override
    {
        std::map<std::string, std::string> metadata;
        std::vector<std::string> columnsValue;
        std::string line, word;

        std::lock_guard<std::mutex> lock(basicsLck);

        std::fstream file;
        file.open(imdbFilename, std::ios::in);

        if(!file.is_open()) {
            std::cerr << typeid(this).name() << " - File is not open and cannot be parsed" << std::endl;
            return metadata;
        }

        assert(!match.second.empty());

        unsigned short matchColumnIndex = match.first;
        std::string matchColumnValue = match.second;

        auto columnStructure = mapBasics;
        file.seekg(currentFilePos, file.beg);

        std::sort(find.begin(), find.end());
        if(currentFilePos == 0) {
            getline(file, line); // ignore header
        }

        while(getline(file, line))
        {
            if(line.find(match.second) == std::string::npos) {
                continue;
            }

            /*
             * divide string into columns
             * */
            std::stringstream ss(line);
            while (getline(ss, word, '\t'))
            {
                columnsValue.emplace_back(word);
            }

            /*
             * Search among our columnns for our matching property i.e. title
             * */
            auto found = std::find(columnsValue.begin(), columnsValue.end(), matchColumnValue);
            if(found != columnsValue.end()) {
                currentFilePos = file.tellg(); // save current position in file so we don't need to parse the entire file for next property
                currentFilePos -= headerSize;
                std::string matching = *found;

                // make pair of i.e. columnIndex (match.first) and our matching property
                metadata.insert ( std::make_pair(columnStructure.at(matchColumnIndex), matching));

                for(auto &filterAdd : find) {
                    unsigned short newColumnIndex = filterAdd.first;
                    assert(columnsValue.size() >= newColumnIndex);
                    assert(columnStructure.size() >= newColumnIndex);
                    auto type = columnStructure.at(newColumnIndex);
                    auto val = columnsValue.at(newColumnIndex);
                    metadata.insert ( std::pair<std::string, std::string> (type, val) );
                }
            }
            columnsValue.resize(0); // clear before next row
        }
        file.close();

        return metadata;
    }
};

#endif //SYNOFW_IMDBBASICS_H
