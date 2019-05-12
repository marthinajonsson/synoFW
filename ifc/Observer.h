

//
// Created by mjonsson on 5/9/19.
//


#ifndef SYNOFW_OBSERVER_H
#define SYNOFW_OBSERVER_H

namespace Pattern{

    class Observer {

    public:

        Observer() = default;

        ~Observer() = default;

        virtual int update(Subject *subject) = 0;

    };
}

#endif //SYNOFW_OBSERVER_H
