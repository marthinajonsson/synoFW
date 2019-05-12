//
// Created by mjonsson on 5/12/19.
//

#include "Subject.h"
#include "Observer.h"
//class Subject;
//class Observer;

void Subject::registerObserver(SeverityType event, Observer *observer) {
    registryMap[event].push_back(observer);
}

void Subject::removeObserver(Observer *observer) {
    for (auto &it : registryMap) {
        auto vecIt = std::find(it.second.begin(), it.second.end(), observer);
        if(vecIt != it.second.end())
        {
            it.second.erase(vecIt);
            continue;
        }
    }
}

void Subject::notifyObservers(SeverityType event) {
    auto it = registryMap.find(event);
    if(it != registryMap.end())
    {
        for(auto vecIt = it->second.begin(); vecIt != it->second.end(); vecIt++)
        {
            (*vecIt)->update(event, this);
        }
    }
}
