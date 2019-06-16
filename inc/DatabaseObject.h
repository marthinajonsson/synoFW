//
// Created by mjonsson on 5/20/19.
//

#ifndef SYNOFW_DATABASEOBJECT_H
#define SYNOFW_DATABASEOBJECT_H

#include <string>

const std::string TITLE = "title";
const std::string TITLE_ID = "titleId";
const std::string TITLE_TYPE = "titleType";
const std::string GENRE = "genre";
const std::string START_YEAR = "startYear";
const std::string END_YEAR = "endYear";
const std::string DIRECTORS = "directors";
const std::string WRITERS = "writes";
const std::string ACTORS = "actors";
const std::string RUNTIME_MINUTES = "runtimeMinutes";
const std::string PARENT_CONST = "parentTconst";
const std::string REGION = "region";
const std::string LANGUAGE = "language";
const std::string EPISODE = "episode";
const std::string SEASON = "season";
const std::string PATH  = "path";
const std::string LIBRARY_ID = "library_id";
const std::string FILENAME = "name";




enum PROPERTY {
    TITLE_P = 0,
    TITLE_ID_P = 1,
    TITLE_TYPE_P = 2,
    GENRE_P,
    START_YEAR_P,
    END_YEAR_P,
    DIRECTORS_P,
    WRITERS_P,
    ACTORS_P,
    RUNTIME_P,
    REGION_P,
    LANGUAGE_P,
    PARENT_CONST_P,
    EPISODE_P,
    SEASON_P,
    PATH_P,
    LIBRARY_ID_P,
    FILENAME_P = 16
};

struct database {
    std::string m_title;
    std::string m_titleId;
    std::string m_titleType = "UNDEF";
    std::string m_genre = "UNDEF";
    std::string m_startYear = "UNDEF";
    std::string m_endYear = "UNDEF";
    std::string m_directors = "UNDEF";
    std::string m_writers = "UNDEF";
    std::string m_actors = "UNDEF";
    std::string m_runtimeMinutes;
    std::string m_region = "UNDEF";
    std::string m_language = "UNDEF";
    std::string m_parentTconst = "UNDEF";
    std::string m_episode = "UNDEF";
    std::string m_season = "UNDEF";
    std::string m_path = "UNDEF";
    std::string m_library_id = "UNDEF";
    std::string m_filename = "UNDEF";
};

#endif //SYNOFW_DATABASEOBJECT_H
