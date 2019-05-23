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
#include <future>
#include <iostream>

class ActiveObject {
private:

    std::string val;
    Queue queue;
    std::atomic<bool> done;
    std::thread *runnable;

public:
    ActiveObject() : val(""), done(false) {
        runnable = new std::thread(&ActiveObject::run, this);
    }
    ~ActiveObject() { runnable->join(); }

    std::string getVal() { return val; }

    void registerWork(std::string choice, std::string arg = "") {
        val = arg;
        queue.put(choice);
    }

    void run() {
        while (!done) {
            queue.take()();
        }
    }
};


#endif //SYNOFW_ACTIVEOBJECT_H
