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
public:
    ActiveObject () : _done(false) {
        _runnable = new boost::thread(&ActiveObject::run, this);
    }
    ~ActiveObject () { _runnable->join(); }

    bool stillRunning () {
        return _runnable->joinable();
    }

    void stop () {
        _done = false;
    }

    void registerRequest (std::string choice) {
        _queue.put(choice);
    }

    void run () {
        while (!_done)
            _queue.take();
    }

private:
    Queue _queue;
    boost::atomic_bool _done;
    boost::thread *_runnable;

};


#endif //SYNOFW_ACTIVEOBJECT_H
