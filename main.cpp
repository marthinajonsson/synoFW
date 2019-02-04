#include <iostream>
#include <sstream>
#include <algorithm>

#include <RequestHandler.h>
#include <FileStationAPI.h>
#include <VideoStationAPI.h>
#include <ErrorCodes.h>

void printOptions() {

    std::cout << "info" << std::endl;
    std::cout << "library" << std::endl;
    std::cout << "movie" << std::endl;
    std::cout << "tvshow" << std::endl;
    std::cout << "folder" << std::endl;
    std::cout << "*metadata TBD.." << std::endl;
    std::cout << "file" << std::endl;
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

    std::string parsed;
    int result = process(parsed);

    if(result == -1) {
        return result;
    }

    if(parsed.find("file") != std::string::npos) {
        FileStationAPI fs;
        fs.makeRequest(parsed);
    }
    else {
        VideoStationAPI vs;
        vs.makeRequest(parsed);
    }
    //
//    FileStationAPI fs;
//
//    std::string url =
//            "http://192.168.0.107:5000//webapi/entry.cgi?api=SYNO.FileStation.Search&version=2&method=start&folder_path=//video&_sid=";
//
//    RequestHandler::getInstance().make(url);


    return 0;
}