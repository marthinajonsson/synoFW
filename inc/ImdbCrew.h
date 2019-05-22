//
// Created by mjonsson on 5/22/19.
//

#ifndef SYNOFW_IMDBCREW_H
#define SYNOFW_IMDBCREW_H


#include <fstream>
#include "Imdb.h"
#include "Subject.h"


class Logger;

class ImdbCrew : Subject, Imdb {
private:
    std::string imdbFilename = "title.crew.tsv";
    long currentFilePos;

    std::map<unsigned short, std::string> mapCrew {
            {0, "titleId"}, {1, "directors"}, {2, "writers"}
    };

public:
    ImdbCrew() : currentFilePos(0){}
    ~ImdbCrew() = default;

    std::map<std::string, std::string> parse(std::pair<unsigned short, std::string> &&match, std::vector<std::pair<unsigned short, std::string>> &&find) override
    {
        std::map<std::string, std::string> metadata;
        std::vector<std::string> columnsValue;
        std::string line, word;

        std::lock_guard<std::mutex> lock(crewLck);

        std::fstream file;
        file.open(imdbFilename, std::ios::in);

        if(!file.is_open()) {
            std::cerr << typeid(this).name() << " - File is not open and cannot be parsed" << std::endl;
            return metadata;
        }

        assert(!match.second.empty());

        unsigned short matchColumnIndex = match.first;
        std::string matchColumnValue = match.second;

        auto columnStructure = mapCrew;
        file.seekg(currentFilePos, file.beg);

        std::sort(find.begin(), find.end());
        getline(file, line); // ignore header

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
                std::string matching = *found;

                // make pair of i.e. columnIndex (match.first) and our matching property
                metadata.insert ( std::make_pair(columnStructure.at(matchColumnIndex), matching));

                for(auto &filterAdd : find) {
                    unsigned short newColumnIndex = filterAdd.first;
                    // make pair of new columnIndex (structure from meta and value from columns)
                    metadata.insert ( std::pair<std::string, std::string> (columnStructure.at(newColumnIndex), columnsValue.at(newColumnIndex)) );
                }
            }
            columnsValue.resize(0); // clear before next row
        }
        file.close();

        return metadata;
    }
};


#endif //SYNOFW_IMDBCREW_H
