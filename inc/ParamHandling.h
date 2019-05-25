//
// Created by mjonsson on 2/8/19.
//

#ifndef SYNOFW_PARAMHANDLING_H
#define SYNOFW_PARAMHANDLING_H

#include <algorithm>
#include <iostream>
#include <string>

class ParamHandling {

public:

    std::string setParam(std::string &&param, std::string &&defaultvalue) {
        std::string value;
        std::cout << "Set param (" + param + "): ";
        std::getline(std::cin>>std::ws, value);
        if(value.empty()) {
            value = defaultvalue;
        }
        return value;
    }

    std::string getPathFolder(std::string& pattern)
    {
        std::transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
        if (pattern.find("old") != std::string::npos) {
            return "/video/film/Old";
        }
        else if (pattern.find("barn") != std::string::npos) {
            return "/video/film/Barn";
        }
        else if(pattern.find("rom") != std::string::npos) {
            return "/video/film/RomAndCom";
        }
        else if(pattern.find("com") != std::string::npos) {
            return "/video/film/RomAndCom";
        }
        else if(pattern.find("horror") != std::string::npos) {
            return "/video/film/Horror";
        }
        else if(pattern.find("king") != std::string::npos) {
            return "/video/film/StephenKing";
        }
        else if(pattern.find("hitch") != std::string::npos) {
            return "/video/film/Old/Hitchcock";
        }
        else if(pattern.find("crime") != std::string::npos) {
            return "/video/film/Crime";
        }
        else if(pattern.find("hallmark") != std::string::npos) {
            return "/video/film/Sliskigt";
        }
        else if(pattern.find("mara") != std::string::npos) {
            return "/video/film/MovieSequels";
        }
        else if(pattern.find("doc") != std::string::npos) {
            return "/video/film/Documentary";
        }
        else if(pattern.find("serie") != std::string::npos) {
            return "/video/series";
        }
        return "/video/film";
    }

    std::string getPath(std::string& pattern)
    {
        std::transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);

        if (pattern.find("old") != std::string::npos) {
            return "/volume1/video/film/Old";
        } else if (pattern.find("barn") != std::string::npos) {
            return "/volume1/video/film/Barn";
        }
        else if(pattern.find("rom") != std::string::npos) {
            return "/volume1/video/film/RomAndCom";
        }
        else if(pattern.find("com") != std::string::npos) {
            return "/volume1/video/film/RomAndCom";
        }
        else if(pattern.find("horror") != std::string::npos) {
            return "/volume1/video/film/Horror";
        }
        else if(pattern.find("king") != std::string::npos) {
            return "/volume1/video/film/StephenKing";
        }
        else if(pattern.find("hitch") != std::string::npos) {
            return "/volume1/video/film/Old/Hitchcock";
        }
        else if(pattern.find("crime") != std::string::npos) {
            return "/volume1/video/film/Crime";
        }
        else if(pattern.find("hallmark") != std::string::npos) {
            return "/volume1/video/film/Sliskigt";
        }
        else if(pattern.find("mara") != std::string::npos) {
            return "/volume1/video/film/MovieSequels";
        }
        else if(pattern.find("doc") != std::string::npos) {
            return "/volume1/video/film/Documentary";
        }
        else if(pattern.find("serie") != std::string::npos) {
            return "/volume1/video/series";
        }
        else if(pattern.find("film") != std::string::npos) {
            return "/volume1/video/film";
        }
        return "";
    }

};


#endif //SYNOFW_PARAMHANDLING_H
