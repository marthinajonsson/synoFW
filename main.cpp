#include <iostream>
#include <sstream>
#include <algorithm>
#include <future>

#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <chrono>
#include <ctime>

#include "Logger.h"
#include "EventLogger.h"
#include "ActiveObject.h"
#include <RequestHandler.h>
#include <CacheMgr.h>
#include <FileStationAPI.h>
#include <VideoStationAPI.h>
#include <ErrorCodes.h>
#include "Utilities.h"

std::shared_ptr pLog = std::make_shared<Logger>();


struct tm *timeS;
struct stat statS;


void download() {

    stat("title.akas.tsv", &statS);
    timeS = gmtime(&(statS.st_mtime));
    std::cout << "Year: " << timeS->tm_year;
    std::cout << "Month: " << timeS->tm_mon;
    std::cout << "Day: " << timeS->tm_mday;

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::cout << std::ctime(&now_time);

}



static bool parseImdb(std::string title, std::shared_ptr<Logger> &logger) {
    CacheMgr::getInstance().validate(title);
    DatabaseObject obj = CacheMgr::getInstance().get(title);
    logger->writeLog(SeverityType::GENERAL, obj.m_title);
    logger->writeLog(SeverityType::GENERAL, obj.m_titleId);
    logger->writeLog(SeverityType::GENERAL, obj.m_titleType);
    logger->writeLog(SeverityType::GENERAL, obj.m_genre);
    logger->writeLog(SeverityType::GENERAL, obj.m_language);
    logger->writeLog(SeverityType::GENERAL, obj.m_runtimeMinutes);
    logger->writeLog(SeverityType::GENERAL, obj.m_directors);
    logger->writeLog(SeverityType::GENERAL, obj.m_writers);
    logger->writeLog(SeverityType::GENERAL, obj.m_startYear);
    logger->writeLog(SeverityType::GENERAL, obj.m_endYear);
    logger->writeLog(SeverityType::GENERAL, obj.m_region);
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
            download();
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
    ActiveObject active;
    //active.registerWork("updateFiles");

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

    std::string result = "Woman.in.Gold.2015.1080p.BluRay.x264.YIFY.mp4";
    auto res = parseImdb(result, pLog);
    pLog->removeObserver(pElog);

    delete pElog;
    return 0;
}