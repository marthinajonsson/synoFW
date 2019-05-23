//
// Created by mjonsson on 5/23/19.
//

#ifndef SYNOFW_IMDBEPISODE_H
#define SYNOFW_IMDBEPISODE_H

#include <fstream>
#include "Imdb.h"


class Logger;

class ImdbEpisode : Imdb {
private:
    std::string imdbFilename = "title.episode.tsv";
    long currentFilePos;
    long headerSize;

    std::map<unsigned short, std::string> mapEpisode {
            {0, "episodeId"}, {1, "parentTconst"}, {2, "season"}, {3, "episode"}
    };

public:
    ImdbEpisode() {
        std::fstream file;
        file.open(imdbFilename, std::ios::in);
        currentFilePos = file.tellg();
        std::string tmp;
        getline(file, tmp);
        headerSize = file.tellg();
        file.seekg (0, file.beg);
        file.close();
    };
    ~ImdbEpisode() = default;

    std::map<std::string, std::string> parse(std::pair<unsigned short, std::string> &&match, std::vector<std::pair<unsigned short, std::string>> &&find) override
    {
        std::map<std::string, std::string> metadata;
        std::vector<std::string> columnsValue;
        std::string line, word;

        std::lock_guard<std::mutex> lock(episodeLck);

        std::fstream file;
        file.open(imdbFilename, std::ios::in);

        if(!file.is_open()) {
            std::cerr << typeid(this).name() << " - File is not open and cannot be parsed" << std::endl;
            return metadata;
        }

        assert(!match.second.empty());

        unsigned short matchColumnIndex = match.first;
        std::string matchColumnValue = match.second;

        auto columnStructure = mapEpisode;
        //file.seekg(currentFilePos, file.beg);

        file.seekg(currentFilePos, file.beg); // -header

        long pos = file.tellg();
        std::sort(find.begin(), find.end());
        if(currentFilePos == 0) {
            getline(file, line); // ignore header
        }

        long pos2 = file.tellg();
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

#endif //SYNOFW_IMDBEPISODE_H
