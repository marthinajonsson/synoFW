//
// Created by mjonsson on 5/2/19.
//

#ifndef SYNOFW_FILENAMESTRUCTURE_H
#define SYNOFW_FILENAMESTRUCTURE_H

#include <boost/assert.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

class FilenameStructure {
public:

    std::string getTitle () { return _title; }
    std::string getYear () { return _year; }

    void parse (std::string &&filename) {
        std::vector<std::string> parts;
        std::string temp;

        BOOST_ASSERT(!filename.empty());
        std::stringstream s(filename);
        while (getline(s, temp, '.')) {
            parts.emplace_back(temp);
        }

        auto resolutionIt = std::find_if(parts.begin(), parts.end(), [](const std::string& s) { return s == "720p" || s == "1080p"; });
        if (resolutionIt != parts.end())
        {
            _resolution = *resolutionIt;

            auto yearIt = resolutionIt-1;
            _year = *yearIt;

            std::ostringstream s;

            std::copy(parts.begin(), yearIt-1,
                      std::ostream_iterator<std::string>(s, " "));


            auto titleIt = yearIt-1;
            s << *titleIt;
            _title = s.str();

            _extension = parts.back();

        }
    }
private:
    std::string _title;
    std::string _year;
    std::string _resolution;
    std::string _extension;

};


#endif //SYNOFW_FILENAMESTRUCTURE_H
