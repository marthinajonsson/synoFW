//
// Created by mjonsson on 2/8/19.
//

#ifndef SYNOFW_PARAMHANDLING_H
#define SYNOFW_PARAMHANDLING_H


#include <string>

class ParamHandling {

public:



    std::string getPath(std::string& pattern) {

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
            return "/volume1/video/film/DocumentaryTalkShow";
        }
        else if(pattern.find("serie") != std::string::npos) {
            return "/volume1/video/series";
        }
        return "";
    }

};


#endif //SYNOFW_PARAMHANDLING_H
