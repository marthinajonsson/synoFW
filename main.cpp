#include <iostream>
#include <sstream>
#include <algorithm>

#include <RequestHandler.h>
#include <FileStationAPI.h>
#include <VideoStationAPI.h>

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

int main(int argc, char* argv []) {

    std::vector<std::string> args;
    std::vector<std::string> TOPIC = {"movie", "series", "file"};

    for (int i = 1; i < argc; ++i) { // Remember argv[0] is the path to the program, we want from argv[1] onwards
        args.push_back(argv[i]); // Add all but the last argument to the vector.

    };

    auto valid = std::find(TOPIC.begin(), TOPIC.end(), args.front());

    if(valid == TOPIC.end()) {
        std::cout << "Valid first arg is movie series or file" << std::endl;
        return 0;
    }

    if(args.front() == TOPIC.back()) {
        FileStationAPI fs;
        fs.setMethod();
    }
    else {
        VideoStationAPI vs;
        vs.setMethod();
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