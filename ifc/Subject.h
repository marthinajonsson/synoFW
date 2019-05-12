//
// Created by mjonsson on 5/9/19.
//

#ifndef SYNOFW_SUBJECT_H
#define SYNOFW_SUBJECT_H

#include <algorithm>
#include <map>
#include <vector>

namespace Pattern{

#include "Observer.h"

    class Subject {

        std::string info;
    public:

        typedef enum SeverityType
        {
            ERROR,
            WARNING,
            GENERAL,
        }SeverityType;

        void setInfo(std::string &value) {
            info = value;
        }

        std::string& getInfo() { return info; }

        void registerObserver(int event, Observer *observer);
        void removeObserver(Observer *observer);
        void notifyObservers(int event);
    };

}

#endif //SYNOFW_SUBJECT_H
