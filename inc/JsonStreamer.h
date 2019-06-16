//
// Created by mjonsson on 5/20/19.
//

#ifndef SYNOFW_JSONREADWRITE_H
#define SYNOFW_JSONREADWRITE_H

#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <map>

#include "DatabaseObject.h"
#include "Utilities.h"

class JsonStreamer {
private:
    std::string _path;
    std::map<PROPERTY, std::string> _propertyMap;

    void writeStream(database &obj);
    boost::property_tree::ptree readStream(std::string&);
public:

    explicit JsonStreamer (const std::string &&filename) {
        _path = "../data/";
        _path = _path.append(filename);

        map_init(_propertyMap)
                (PROPERTY::TITLE_P, TITLE)
                (PROPERTY::TITLE_ID_P, TITLE_ID)
                (PROPERTY::TITLE_TYPE_P, TITLE_TYPE)
                (PROPERTY::GENRE_P, GENRE)
                (PROPERTY::START_YEAR_P, START_YEAR)
                (PROPERTY::END_YEAR_P, END_YEAR)
                (PROPERTY::DIRECTORS_P, DIRECTORS)
                (PROPERTY::WRITERS_P, WRITERS)
                (PROPERTY::ACTORS_P, ACTORS)
                (PROPERTY::RUNTIME_P, RUNTIME_MINUTES)
                (PROPERTY::REGION_P, REGION)
                (PROPERTY::LANGUAGE_P, LANGUAGE)
                (PROPERTY::PARENT_CONST_P, PARENT_CONST)
                (PROPERTY::EPISODE_P, EPISODE)
                (PROPERTY::SEASON_P, SEASON)
                (PROPERTY::PATH_P, PATH)
                (PROPERTY::LIBRARY_ID_P, LIBRARY_ID)
                (PROPERTY::FILENAME_P, FILENAME)
                ;
    };

    ~JsonStreamer() = default;

    database find(std::string&);
    void update(database &obj);
    bool checkForNull(std::string&);
};


#endif //SYNOFW_JSONREADWRITE_H
