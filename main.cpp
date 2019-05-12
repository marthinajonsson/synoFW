#include <iostream>
#include <sstream>
#include <algorithm>
#include <future>

#include "Logger.h"
#include "EventLogger.h"

#include <FilenameStructure.h>
#include <RequestHandler.h>
#include <ImdbStructure.h>
#include <FileStationAPI.h>
#include <VideoStationAPI.h>
#include <ErrorCodes.h>
#include "Utilities.h"

std::shared_ptr pLog = std::make_shared<Logger>();

static void downloadImdb() {
    ImdbStructure::getInstance().fetch(pLog);
}

static void parseImdb(std::string title) {
    ImdbStructure::getInstance().parseTitle(pLog, title);
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


int process(std::string &parsed)
{
    std::future<void> fut;
    for (std::string line; std::cout << "SYNO > " && std::getline(std::cin, line); )
    {
        if (line.find("help") != std::string::npos) {
            printOptions();
        }
        else if (line.find("exit") != std::string::npos) {
            pLog->writeLog(SeverityType::GENERAL, "Exiting application.. ");
            return 0;
        }
        else if(line.find("update") != std::string::npos) {

            pLog->writeLog(SeverityType::GENERAL, "Download information asynchronic.. ");
            fut = std::async(std::launch::async, downloadImdb);
        }
        else if (!line.empty()) {
            parsed = line;
            return 0;
        }
        if(fut.valid()) {
            pLog->writeLog(SeverityType::GENERAL, "Waiting for download to complete.. ");
            fut.get();
            pLog->writeLog(SeverityType::GENERAL, "Download completed");
        }
    }
    return -1;
}


int main(int argc, char* argv [])
{

    EventLogger *pElog = new EventLogger();
    pLog->registerObserver(SeverityType::GENERAL, pElog);
    pLog->registerObserver(SeverityType::WARNING, pElog);
    pLog->registerObserver(SeverityType::ERROR, pElog);
    pLog->writeLog(SeverityType::GENERAL, "Fetch SYNO information.. ");
//    std::string input;
//    int result = process(input);
//
//    if(result == -1) {
//        return result;
//    }
//
//    auto parsed = split(input, ':');
//    auto app = parsed.front();
//    pop_front(parsed);
//    input = parsed.front();
//
//    if(app.find("fs") != std::string::npos) {
//        FileStationAPI fs;
//        fs.makeRequest(input);
//    }
//    else if(app.find("vs") != std::string::npos) {
//        VideoStationAPI vs;
//        vs.makeRequest(input);
//    }
//    else {
//        printOptions();
//    }


/*
 * FROM SYNO REQUEST FETCH INFORMATION ABOUT GIVEN TITLE
 *
 */

    FilenameStructure fnStr;
    fnStr.parse("Woman.in.Gold.2015.1080p.BluRay.x264.YIFY.mp4");
    std::string title = fnStr.getTitle();

    auto fut = std::async(std::launch::async, parseImdb, title);
    fut.get();

    pLog->removeObserver(pElog);

    delete pElog;
    return 0;
}