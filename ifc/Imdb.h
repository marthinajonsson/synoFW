//
// Created by mjonsson on 5/21/19.
//

#ifndef SYNOFW_IMDBPARSER_H
#define SYNOFW_IMDBPARSER_H


#include "Logger.h"
#include "DatabaseObject.h"

#include <string>
#include <mutex>

class Imdb {
public:
    Imdb() = default;
    ~Imdb() = default;

    template<typename T>
    T parse(std::pair<unsigned short, std::string> &&match, std::vector<std::pair<unsigned short, std::string>> &&find);
};


#endif //SYNOFW_IMDBPARSER_H
