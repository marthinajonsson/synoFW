#include <iostream>
#include <sstream>
#include <algorithm>

#include <RequestHandler.h>
#include <FileStationAPI.h>
#include <VideoStationAPI.h>
#include <ErrorCodes.h>
#include "Utilities.h"

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
    for (std::string line; std::cout << "SYNO > " && std::getline(std::cin, line); )
    {
        if (line.find("help") != std::string::npos) {
            printOptions();
        }
        else if (line.find("exit") != std::string::npos) {
            return 0;
        }
        else if (!line.empty()) {
            parsed = line;
            return 0;
        }
    }
    return -1;
}


int main(int argc, char* argv []) {

    std::string input;
    int result = process(input);

    if(result == -1) {
        return result;
    }

    auto parsed = split(input, ':');
    auto app = parsed.front();
    pop_front(parsed);
    input = parsed.front();

    if(app.find("fs") != std::string::npos) {
        FileStationAPI fs;
        fs.makeRequest(input);
    }
    else if(app.find("vs") != std::string::npos) {
        VideoStationAPI vs;
        vs.makeRequest(input);
    }
    else {
        printOptions();
    }

    return 0;
}