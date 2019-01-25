#include <iostream>

#include <RequestHandler.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string url =
            "http://192.168.0.107:5000/webapi/VideoStation/movie.cgi?api=SYNO.VideoStation.Movie&version=2&method=list&sid=";
    RequestHandler::getInstance().make(url);

    return 0;
}