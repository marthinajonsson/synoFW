//
// Created by mjonsson on 1/27/19.
//

#ifndef SYNOFW_FILESTATIONAPI_H
#define SYNOFW_FILESTATIONAPI_H

#include <string>
#include <map>

#include <json/json.h>
#include <fstream>

#include "Utilities.h"



class FileStationAPI {
public:
    FileStationAPI() {
       load();
    }
    ~FileStationAPI() = default;

    void setMethod();

private:

    void load();

};


#endif //SYNOFW_FILESTATIONAPI_H
