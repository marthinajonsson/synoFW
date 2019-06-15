//
// Created by mjonsson on 5/20/19.
//

#ifndef SYNOFW_DATABASEOBJECT_H
#define SYNOFW_DATABASEOBJECT_H

#include <string>

struct database {
    std::string m_title;
    std::string m_titleId;
    std::string m_titleType;
    std::string m_genre;
    std::string m_startYear;
    std::string m_endYear;
    std::string m_directors;
    std::string m_writers;
    std::string m_runtimeMinutes;
    std::string m_region;
    std::string m_language;
    std::string m_parentTconst;
    std::string m_episode;
    std::string m_season;
    std::string m_path;
    std::string m_library_id;
    std::string m_filename;
};

#endif //SYNOFW_DATABASEOBJECT_H
