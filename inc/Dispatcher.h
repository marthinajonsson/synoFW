//
// Created by mjonsson on 5/23/19.
//

#ifndef SYNOFW_DISPATCHER_H
#define SYNOFW_DISPATCHER_H

#include <string>
#include <functional>

#include "FileMgr.h"
#include "FileStationAPI.h"
#include "VideoStationAPI.h"

/*
 *  vs:info -- get info
 *  vs:library -- list all libraries
 *  vs:movie
 *  vs:tvshow
 *  vs:folder
 *
 *  fs:info -- get info
 *  fs:list -- list shared folders or files
 *
 */

class Dispatcher {
public:

    std::function<void()> set (std::string &request)
    {
        if (request == "update") {
            return std::bind(&FileMgr::fetch, &fMgr);
        }
        else if (request.find("vs") != std::string::npos) {
            VideoStationAPI vs;
            return std::bind(&VideoStationAPI::makeRequest, &vs, request);
        }
        else if (request.find("fs") != std::string::npos) {
            FileStationAPI fs;
            return std::bind(&FileStationAPI::makeRequest, &fs, request);
        }
        return [] () { };
    }

private:
    FileMgr fMgr;
};


#endif //SYNOFW_DISPATCHER_H
