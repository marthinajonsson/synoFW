//
// Created by mjonsson on 5/9/19.
//

#ifndef SYNOFW_SUBJECT_H
#define SYNOFW_SUBJECT_H

#include "Utilities.h"

#include <algorithm>
#include <map>
#include <vector>

class Observer;

class Subject
{
private:
    std::string info;
    std::map<int , std::vector<Observer *> > registryMap;
public:

    void setInfo(std::string &value) { info = value; }
    std::string& getInfo() { return info; }

    void registerObserver(SeverityType event, Observer *observer);
    void removeObserver(Observer *observer);
    void notifyObservers(SeverityType event);
};


#endif //SYNOFW_SUBJECT_H
