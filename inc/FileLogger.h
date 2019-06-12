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
        std::string fName = getFilename();
        logfile.open(fName, std::ios::out | std::ios::app);
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
        std::string filename = "FileLogger";
        filename.append("-");
        filename.append(date);
        filename.append(".log");
        boost::filesystem::path cwd(boost::filesystem::current_path());
        filename = cwd.string() + "/logs/" + filename;
        boost::filesystem::path dir(cwd.string() + "/logs/");
        if(!(boost::filesystem::exists(cwd.string() + "/logs/"))){
            if (!boost::filesystem::create_directory(cwd.string() + "/logs/"))
                std::cerr << "Failed to create log folder" << std::endl;
        }
        return filename;
    }

    void write(SeverityType level, std::string &output) {
        std::lock_guard<std::mutex> lock(m_write);

        if (!logfile.is_open()){
            std::cerr << "Log file is not open\n";
            return;
        }

        long begin = logfile.tellp();
        logfile.seekp(0, std::ios::end);
        long end = logfile.tellp();

        if((end- begin) > 1024) {
            logfile << std::flush;
        }

        std::string time = getTime();
        std::string extras =  __FILE__;
        extras.append(":");
        extras.append(std::to_string(__LINE__));
        extras.append("] ");
        time = "[" + extras + " ";
        logfile << time + output + "\n";
    }
};
#endif //SYNOFW_FILELOGGER_H
