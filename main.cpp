#include <iostream>

#include <RequestHandler.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string url =
            "http://192.168.0.107:5000/webapi/VideoStation/info.cgi?api=SYNO.VideoStation.Info&version=1&method=info";
            //getinfo&sid=";
    RequestHandler::getInstance().make(url);

    return 0;
}