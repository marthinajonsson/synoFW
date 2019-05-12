//
// Created by mjonsson on 5/9/19.
//

#ifndef SYNOFW_LOGGER_H
#define SYNOFW_LOGGER_H

#include <Subject.h>

#include <iostream>
#include <map>
#include <ctime>

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
