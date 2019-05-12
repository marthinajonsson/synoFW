

//
// Created by mjonsson on 5/9/19.
//


#ifndef SYNOFW_OBSERVER_H
#define SYNOFW_OBSERVER_H

#include "Utilities.h"

class Subject;

class Observer {

public:

    Observer() = default;

    ~Observer() = default;

    virtual void update(SeverityType event, Subject *subject) = 0;

};


#endif //SYNOFW_OBSERVER_H
