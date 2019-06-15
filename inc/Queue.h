//
// Created by mjonsson on 5/23/19.
//

#ifndef SYNOFW_QUEUE_H
#define SYNOFW_QUEUE_H


#include "Dispatcher.h"

#include <queue>
#include <mutex>

typedef std::function<void()> Request;

class Queue {
public:

    void put (std::string &val) {
        std::lock_guard<std::mutex> guard(_lck);
        Request request = _dispatcher.set(val);
        _queue.push(request);
        _empty.notify_one();
    }

    Request take() {
        std::unique_lock<std::mutex> lock(_lck);
        _empty.wait(lock, [&]{ return !_queue.empty(); });
        Request request = _queue.front();
        _queue.pop();
        return request;
    }

private:
    std::mutex _lck;
    std::queue<Request> _queue;
    std::condition_variable _empty;
    Dispatcher _dispatcher;
};


#endif //SYNOFW_QUEUE_H
