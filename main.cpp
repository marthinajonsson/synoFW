#include <iostream>

#include <RequestHandler.h>
#include <FileStationAPI.h>

int main() {
    std::cout << "Hello, World!" << std::endl;
    FileStationAPI fs;

    std::string url =
            "http://192.168.0.107:5000//webapi/entry.cgi?api=SYNO.FileStation.Search&version=2&method=start&folder_path=//video&_sid=";

    RequestHandler::getInstance().make(url);


    return 0;
}