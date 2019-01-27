#include <iostream>

#include <RequestHandler.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string url =
            "http://192.168.0.107:5000//webapi/entry.cgi?api=SYNO.FileStation.Info&version=2&method=get&_sid=";
    RequestHandler::getInstance().make(url);

    return 0;
}