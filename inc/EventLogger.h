//
// Created by mjonsson on 5/9/19.
//

#ifndef SYNOFW_STATUSLOGGER_H
#define SYNOFW_STATUSLOGGER_H

#include "Utilities.h"
#include "Subject.h"
#include "Observer.h"

#include <fstream>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <mutex>
#include <map>

#define GetCurrentDir getcwd

static std::mutex readwrite;

class EventLogger : public Observer {
public:

    EventLogger() {
        logfile.open(getFilename(), std::ios::out | std::ios::app );
    }

    ~EventLogger() {
        logfile.close();
    }

    void update(SeverityType event, Subject *subject) override {
        std::string info = subject->getInfo();
        write(event, info);
    }
    
private:

    std::ofstream logfile;

    std::string getFilename () {
        auto date = getDate();
        std::string filename = typeid(EventLogger).name();
        filename.append("-");
        filename.append(date);
        filename.append(".log");
        auto dir = GetCurrentWorkingDir();
        filename = dir + "/logs/" + filename;
        return filename;
    }

    std::string GetCurrentWorkingDir() {
        char buff[FILENAME_MAX];
        GetCurrentDir( buff, FILENAME_MAX );
        std::string current_working_dir(buff);
        return current_working_dir;
    }


    std::string getDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        auto year = 1900 + ltm->tm_year;
        auto month = 1 + ltm->tm_mon;
        auto day = ltm->tm_mday;
        std::string y = std::to_string(year);
        std::string m = std::to_string(month);
        std::string d = std::to_string(day);
        y = y.append("-");
        m = m.append("-");
        return y + m + d;
    }


    std::string getTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int hour = 1 + ltm->tm_hour;
        auto min = 1 + ltm->tm_min;
        auto sec = 1 + ltm->tm_sec;
        std::string h = std::to_string(hour);
        std::string m = std::to_string(min);
        std::string s = std::to_string(sec);
        h = h.append(":");
        m = m.append(":");
        return h + m + s;
    }

    void write(SeverityType level, std::string &output){
        std::lock_guard<std::mutex> lock(readwrite);
        std::string startcode = "\033[";
        std::string endcode = "\033[0m\n";
        std::string time = getTime();
        time = "[" + time + "]";

        if (level == SeverityType::ERROR) {

            auto thecode = "1;31m";
            std::cerr << startcode + thecode + time + output + endcode << std::endl;
        }
        else if (level == SeverityType::WARNING) {

            auto thecode = "1;33m";
            std::cout << startcode + thecode + time + output + endcode << std::endl;
        }else {
            std::cout << time + output << std::endl;
        }
        std::cout.flush();
    }
};


#endif //SYNOFW_STATUSLOGGER_H
