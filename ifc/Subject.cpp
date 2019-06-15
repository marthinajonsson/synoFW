//
// Created by mjonsson on 5/12/19.
//

#include "Subject.h"
#include "Observer.h"



void Subject::registerObserver(SeverityType event, Observer *observer) {
    _registryMap[event].push_back(observer);
}

void Subject::removeObserver(Observer *observer) {
    for (auto &it : _registryMap) {
        auto vecIt = std::find(it.second.begin(), it.second.end(), observer);
        if(vecIt != it.second.end())
        {
            it.second.erase(vecIt);
            continue;
        }
    }
}

void Subject::notifyObservers(SeverityType event) {
    auto it = _registryMap.find(event);
    if(it != _registryMap.end())
    {
        for(auto & vecIt : it->second)
        {
            vecIt->update(event, this);
        }
    }
}
