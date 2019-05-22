//
// Created by mjonsson on 5/20/19.
//

#ifndef SYNOFW_DATABASEOBJECT_H
#define SYNOFW_DATABASEOBJECT_H

#include <string>

class DatabaseObject {
public:
    std::string m_title;
    std::string m_titleId;
    std::string m_genre;
    std::string m_startYear;
    std::string m_endYear;
    std::string m_directors;
    std::string m_writers;
    std::string m_region;
    std::string m_language;
    std::string m_episode;
    std::string m_season;
    std::string m_crew;
};

#endif //SYNOFW_DATABASEOBJECT_H
