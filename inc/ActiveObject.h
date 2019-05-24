//
// Created by mjonsson on 5/23/19.
//

#ifndef SYNOFW_ACTIVEOBJECT_H
#define SYNOFW_ACTIVEOBJECT_H

#include "json/json.h"
#include "Queue.h"
#include <string>
#include <thread>
#include <queue>
#include <mutex>
//#include <future>
#include <iostream>


class ActiveObject {
private:

    Queue queue;
    std::atomic<bool> done;
    std::thread *runnable;

public:
    ActiveObject() : done(false) {
        runnable = new std::thread(&ActiveObject::run, this);
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
