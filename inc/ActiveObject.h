//
// Created by mjonsson on 5/23/19.
//

#ifndef SYNOFW_ACTIVEOBJECT_H
#define SYNOFW_ACTIVEOBJECT_H

#include "Queue.h"
#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <string>

class ActiveObject {
private:

    Queue queue;
    boost::atomic_bool done;
    boost::thread *runnable;

public:
    ActiveObject() : done(false) {
        runnable = new boost::thread(&ActiveObject::run, this);
    }
    ~ActiveObject() { runnable->join(); }

    bool stillRunning() {
        return runnable->joinable();
    }

    void stop() {
        done = false;
    }

    void registerRequest(std::string choice) {
        queue.put(choice);
    }

    void run() {
        while (!done) {
            queue.take();
        }
    }
};


#endif //SYNOFW_ACTIVEOBJECT_H
