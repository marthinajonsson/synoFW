//
// Created by mjonsson on 5/12/19.
//

#ifndef SYNOFW_FILELOGGER_H
#define SYNOFW_FILELOGGER_H


#include <boost/filesystem.hpp>
#include "Utilities.h"
#include "Subject.h"
#include "Observer.h"

#include <fstream>
#include <iostream>
#include <mutex>
#include <map>


static std::mutex m_write;


class FileLogger : public Observer {
public:

    FileLogger() {

        logfile.open(getFilename(), std::ios::out | std::ios::app);
    }

    ~FileLogger() {
        logfile.close();
    }

    void update(SeverityType event, Subject *subject) override {
        std::string info = subject->getInfo();
        write(event, info);
    }



private:

    std::ofstream logfile;

    std::string getFilename() {
        auto date = getDate();
        std::string filename = typeid(FileLogger).name();
        filename.append("-");
        filename.append(date);
        filename.append(".log");
        boost::filesystem::path cwd(boost::filesystem::current_path());
        filename = cwd.string() + "/logs/" + filename;
        return filename;
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

    void write(SeverityType level, std::string &output) {
        std::lock_guard<std::mutex> lock(m_write);
        std::string time = getTime();
        std::string extras =  __FILE__;
        extras.append(":");
        extras.append(std::to_string(__LINE__));
        extras.append("] ");
        time = "[" + time + ", ";
        logfile << time +  __FILE__ + ":" + std::to_string(__LINE__) + "] " + output + "\n";
    }
};
#endif //SYNOFW_FILELOGGER_H
