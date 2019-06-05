#include <iostream>
#include <sstream>
#include <algorithm>
#include <future>
#include <include/gmock/gmock.h>

#include "CacheTest.h"
#include "FileStationTest.h"
#include "FsRequestTest.h"
#include "VsRequestTest.h"

#include "Logger.h"
#include "EventLogger.h"
#include "FileLogger.h"
#include "ActiveObject.h"
#include <RequestHandler.h>
#include <CacheMgr.h>
#include <FileStationAPI.h>
#include <VideoStationAPI.h>
#include <ErrorCodes.h>
#include "Utilities.h"

#define LOG(x) {std::cout << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ << "): " << x << std::endl;}

std::shared_ptr pLog = std::make_shared<Logger>();

static bool parseImdb(std::string title) {
    CacheMgr::getInstance().validate(title);
    DatabaseObject obj = CacheMgr::getInstance().get(title);
    return true;
}

void printOptions() {

    std::cout << bold_on << "VIDEO STATION" << bold_off << std::endl;
    std::cout << "vs:info" << "-- get info" << std::endl;
    std::cout << "vs:library" << "-- list all libraries" << std::endl;
    std::cout << "vs:movie" << std::endl;
    std::cout << "vs:tvshow" << std::endl;
    std::cout << "vs:folder" << std::endl;
    std::cout << "*vs:metadata TBD.." << std::endl;

    std::cout << bold_on << "FILE STATION" << bold_off << std::endl;
    std::cout << "fs:info" << "-- get info" << std::endl;
    std::cout << "fs:list" << "-- list shared folders or files" << std::endl;
}


int process()
{
    ActiveObject active;
    for (std::string line; std::cout << "SYNO > " && std::getline(std::cin, line); )
    {
        if (line.find("help") != std::string::npos) {
            printOptions();
        }
        else if (line.find("exit") != std::string::npos) {
            active.stop();
            pLog->writeLog(SeverityType::GENERAL, "Exiting application.. ");
            return 0;
        }
        else if(line.find("update") != std::string::npos) {
            pLog->writeLog(SeverityType::GENERAL, "Download information asynchronic.. ");
            active.registerRequest("update");
        }
        else if (!line.empty()) {
            active.registerRequest(line);
            return 0;
        }
    }
    active.stop();

    if(active.stillRunning()) {
       pLog->writeLog(SeverityType::ERROR, "Active thread still running");
    }
    return -1;
}


int main(int argc, char* argv [])
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();

//
//    EventLogger *pElog = new EventLogger();
//    pLog->registerObserver(SeverityType::GENERAL, pElog);
//    pLog->registerObserver(SeverityType::WARNING, pElog);
//    pLog->registerObserver(SeverityType::ERROR, pElog);
//
//    int result = process();
//
//    if(result == -1) {
//        pLog->removeObserver(pElog);
//        delete pElog;
//        return result;
//    }
//
//    std::string test = "Woman.in.Gold.2015.1080p.BluRay.x264.YIFY.mp4";
//
//    delete pElog;
    return 0;
}