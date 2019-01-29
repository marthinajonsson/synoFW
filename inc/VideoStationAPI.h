//
// Created by mjonsson on 1/27/19.
//

#ifndef SYNOFW_VIDEOSTATIONAPI_H
#define SYNOFW_VIDEOSTATIONAPI_H

#include <json/json.h>
#include <fstream>
#include <string>
#include <map>

#include "Utilities.h"



class VideoStationAPI {
public:
    VideoStationAPI() {
       load();
    }
    ~VideoStationAPI() = default;

    void setMethod();

private:

    void load();

};


#endif //SYNOFW_VIDEOSTATIONAPI_H
