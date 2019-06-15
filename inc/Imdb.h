//
// Created by mjonsson on 6/15/19.
//

#ifndef SYNOFW_IMDB_H
#define SYNOFW_IMDB_H

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <mutex>
#include <vector>
#include <map>

class Imdb {
public:
    Imdb () = default;
    ~Imdb () = default;

    void loadfile(const char *filename)
    {
        std::fstream file;
        std::string tmp;
        _imdbFilename = filename;
        file.open(_imdbFilename, std::ios::in);
        _currentFilePos = file.tellg();
        getline(file, tmp);
        _headerSize = file.tellg();
        file.seekg (0, file.beg);
        file.close();
    }

    template <class T>
    T getMap() {
        if (_imdbFilename.find("akas") != std::string::npos)
            return {   {0, "titleId"}, {1, "ordering"}, {2, "title"}, {3, "region"}, {4, "language"} };

        else if (_imdbFilename.find("title.basics") != std::string::npos)
            return { {0, "titleId"}, {1, "titleType"}, {2, "primaryTitle"}, {3, "originalTitle"}, {5, "startYear"}, {6, "endYear"}, {7, "runtimeMinutes"}, {8, "genre"} };

        else if (_imdbFilename.find("crew") != std::string::npos)
            return { {0, "titleId"}, {1, "directors"}, {2, "writers"} };

        else if (_imdbFilename.find("epsiode") != std::string::npos)
            return { {0, "episodeId"}, {1, "parentTconst"}, {2, "season"}, {3, "episode"} };

        else if (_imdbFilename.find("name") != std::string::npos)
            return { {0, "nameId"}, {1, "primaryName"} };

        return {{0,""}};
    }

    template <class T>
    T parse (std::pair<unsigned short, std::string> &&match, std::vector<std::pair<unsigned short, std::string>> &&find)
    {
        T result;
        std::vector<std::string> columnsValue;
        std::string line;
        std::string word;

        std::lock_guard<std::mutex> lock(_lck);

        std::fstream file;
        file.open(_imdbFilename, std::ios::in);

        if(!file.is_open()) {
            std::cerr << typeid(this).name() << " - File is not open and cannot be parsed" << std::endl;
            return result;
        }

        BOOST_ASSERT(!match.second.empty());

        unsigned short matchColumnIndex = match.first;
        std::string matchColumnValue =  match.second;
        boost::algorithm::to_lower(matchColumnValue);

        auto columnStructure = getMap <std::map<unsigned short, std::string>> ();
        file.seekg(_currentFilePos, file.beg); // -header

        long pos = file.tellg();
        std::sort(find.begin(), find.end());
        if(_currentFilePos == 0) {
            getline(file, line); // ignore header
        }

        long pos2 = file.tellg();
        while(getline(file, line))
        {
            boost::algorithm::to_lower(line);
            if(line.find(matchColumnValue) == std::string::npos) {
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
                _currentFilePos = file.tellg(); // save current position in file so we don't need to parse the entire file for next property
                _currentFilePos -= _headerSize;
                std::string matching = *found;

                // make pair of i.e. columnIndex (match.first) and our matching property
                result.insert ( std::make_pair(columnStructure.at(matchColumnIndex), matching));

                for(auto &filterAdd : find) {
                    unsigned short newColumnIndex = filterAdd.first;
                    BOOST_ASSERT(columnsValue.size() >= newColumnIndex);
                    BOOST_ASSERT(columnStructure.size() >= newColumnIndex);
                    auto type = columnStructure.at(newColumnIndex);
                    auto val = columnsValue.at(newColumnIndex);
                    result.insert ( std::pair<std::string, std::string> (type, val) );
                }
            }
            columnsValue.resize(0); // clear before next row
        }
        file.close();

        return result;
    }

private:
    std::string _imdbFilename;
    long _currentFilePos;
    long _headerSize;
    std::mutex _lck;
};


#endif //SYNOFW_IMDB_H
