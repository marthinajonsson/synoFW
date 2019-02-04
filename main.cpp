#include <iostream>
#include <sstream>
#include <algorithm>

#include <RequestHandler.h>
#include <FileStationAPI.h>
#include <VideoStationAPI.h>
#include <ErrorCodes.h>

std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(std::move(token));
    }
    return tokens;
}

void printOptions() {

    std::cout << "info" << std::endl;
    std::cout << "library" << std::endl;
    std::cout << "movie" << std::endl;
    std::cout << "tvshow" << std::endl;
    std::cout << "folder" << std::endl;
    std::cout << "file:list" << std::endl;
}


int process(std::vector<std::string> &parsed)
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
            parsed = split(line, ':');
            return 0;
        }
    }
    return -1;
}


int main(int argc, char* argv []) {

    std::vector<std::string> parsed;
    int result = process(parsed);

    if(result == -1) {
        return result;
    }

    if(parsed.front() == "file") {
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