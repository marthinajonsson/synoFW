//
// Created by mjonsson on 5/9/19.
//

#ifndef SYNOFW_LOGGER_H
#define SYNOFW_LOGGER_H

#include "Utilities.h"
#include "Subject.h"

#include <iostream>
#include <map>
#include <ctime>
#include <functional>

class Logger : public Subject {
public:

    std::map<SeverityType, int> messageMap;

    Logger()
    {
        messageMap[ERROR] = true;
        messageMap[WARNING] = true;
        messageMap[GENERAL] = true;
    }

    ~Logger() = default;

    void writeLog(SeverityType type, std::string info)
    {
        if(messageMap[type])
        {
            setInfo(info);
            notifyObservers(type);
        }
    }

    void writeFile(SeverityType type, const char* file, const char* line, std::string info)
    {
        std::string f (file);
        std::string l (line);
        std::string log = "[";
        log.append(file);
        log.append(", line: ");
        log.append(line);
        log.append("] ");
        log.append(info);
        if(messageMap[type])
        {
            setInfo(log);
            notifyObservers(type);
        }
    }


    void enableLoggingOfMessage(SeverityType type)
    {
        messageMap[type] = true;
    }

    void disableLoggingOfMessage(SeverityType type)
    {
        messageMap[type] = false;
    }
};

#endif //SYNOFW_LOGGER_H
