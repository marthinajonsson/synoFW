//
// Created by mjonsson on 5/2/19.
//

#ifndef SYNOFW_FILENAMESTRUCTURE_H
#define SYNOFW_FILENAMESTRUCTURE_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

class FilenameStructure {
private:
    std::string title;
    std::string year;
    std::string resolution;
    std::string extension;

public:

    std::string getTitle () { return title; }

    void parse(std::string &&filename) {
        std::vector<std::string> parts;
        std::string temp;

        std::stringstream s(filename);
        while (getline(s, temp, '.')) {
            parts.emplace_back(temp);
        }

        auto resolutionIt = std::find_if(parts.begin(), parts.end(), [](const std::string& s) { return s == "720p" || s == "1080p"; });
        if (resolutionIt != parts.end())
        {
            resolution = *resolutionIt;

            auto yearIt = resolutionIt-1;
            year = *yearIt;

            std::ostringstream s;

            std::copy(parts.begin(), yearIt-1,
                      std::ostream_iterator<std::string>(s, " "));


            auto titleIt = yearIt-1;
            s << *titleIt;
            title = s.str();

            extension = parts.back();

        }
    }
};





//struct {
//    unsigned int widthValidated : 1;
//    unsigned int heightValidated : 1;
//} status;
#endif //SYNOFW_FILENAMESTRUCTURE_H
