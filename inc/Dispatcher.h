//
// Created by mjonsson on 5/23/19.
//

#ifndef SYNOFW_DISPATCHER_H
#define SYNOFW_DISPATCHER_H

#include <string>
#include <functional>
#include "FileMgr.h"

class Dispatcher {
private:

public:

    std::function<void()> set(std::string &type)
    {
        FileMgr fMgr;
        if(type == "updateFiles"){
            return std::bind(&FileMgr::fetch, &fMgr);
        }
        else if(type == "downloadAkas") {
            return std::bind(&FileMgr::getDataFiles, &fMgr, "title.akas.tsv.gz");
        }
        else if(type == "downloadBasics") {
            return std::bind(&FileMgr::getDataFiles, &fMgr, "title.basics.tsv.gz");
        }
        else if(type == "downloadCrew") {
            return std::bind(&FileMgr::getDataFiles, &fMgr, "title.crew.tsv.gz");
        }
        else if(type == "downloadEpisode") {
            return std::bind(&FileMgr::getDataFiles, &fMgr, "title.episode.tsv.gz");
        }
        else if(type == "downloadName") {
            return std::bind(&FileMgr::getDataFiles, &fMgr, "name.basics.tsv.gz");
        }
        else {
            return std::bind(&FileMgr::unpackFile, &fMgr, "title.akas.tsv.gz");
        }

        //return std::bind(&MeanReversion::run, &MeanReversion::getInstance());
    }

};


#endif //SYNOFW_DISPATCHER_H
