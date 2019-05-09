//
// Created by mjonsson on 5/9/19.
//

#ifndef SYNOFW_STATUSLOGGER_H
#define SYNOFW_STATUSLOGGER_H

#include "ifc/Logger.h"

#include <fstream>
#include <iostream>
#include <chrono>
#include <unistd.h>
#define GetCurrentDir getcwd

class StatusLogger : Logger{
public:

    StatusLogger() {
        logfile.open(getFilename(), std::ios::out | std::ios::app );
    }
    ~StatusLogger() {
        logfile.close();
    }

    void TRACE(Level_e level, std::string message, std::string errorCode) override{
        std::string statusLevel  = m_levelMap.at(level);
        std::string output = statusLevel + message + " (" + errorCode + ") ";
        write(level, output);
    }

    void TRACE(Level_e level, std::string message) override{
        std::string statusLevel  = m_levelMap.at(level);
        std::string output = statusLevel + message;
        write(level, output);
    }

private:
    /*! \private logFile
     * \var logfile
     * @brief file used by std::ofstream
     */
    std::ofstream logfile;

    /*! \private getFilename
     * @brief gather info from @private getDate() and @private getTime() and put together a filename
     * @return filename in std::string
     */
    std::string getFilename () {
        auto date = getDate();
        std::string filename = date;
        filename.append(".log");
        auto dir = GetCurrentWorkingDir();
        filename = dir + "/logs/" + filename;
        return filename;
    }

    /*! \private GetCurrentWorkingDir
     * @brief used by @private getFilename
     * @return current working directory in std::string
     */
    std::string GetCurrentWorkingDir() {
        char buff[FILENAME_MAX];
        GetCurrentDir( buff, FILENAME_MAX );
        std::string current_working_dir(buff);
        return current_working_dir;
    }

    /*! \private write
     * @brief writes log trace to log file, stdout or stderr
     * @param level @enum Level_e
     * @param output compiled message
     */
    void write(Level_e level, std::string &output){
        std::string startcode = "\033[";
        std::string endcode = "\033[0m\n";
        std::string time = getTime();
        time = "[" + time + "]";

        if (level == ERR) {

            auto thecode = "1;31m";
            std::cerr << startcode + thecode + time + output + endcode << std::endl;
        }
        else if (level == WARN) {

            auto thecode = "1;33m";
            std::cout << startcode + thecode + time + output + endcode << std::endl;
        }

        logfile << time + output + "\n";
    }
};


#endif //SYNOFW_STATUSLOGGER_H
