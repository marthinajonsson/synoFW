//
// Created by mjonsson on 5/9/19.
//

#ifndef SYNOFW_OBSERVER_H
#define SYNOFW_OBSERVER_H

#include <string>
#include <vector>


class Observer {

public:

    Observer() = default;
    ~Observer() = default;


    /**
     * Update the state of this observer
     * @param Request of update
     */
    virtual int update(int&) = 0;
};

#endif //SYNOFW_OBSERVER_H
