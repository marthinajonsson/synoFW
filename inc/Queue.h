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
private:
    std::mutex queueLck;
    std::queue<Request> queue;
    std::condition_variable empty;
    Dispatcher dispatcher;
public:

    void put(std::string &val) {
        std::lock_guard<std::mutex> guard(queueLck);
        Request request = dispatcher.set(val);
        queue.push(request);
        empty.notify_one();
    }

    Request take() {
        std::unique_lock<std::mutex> lock(queueLck);
        empty.wait(lock, [&]{ return !queue.empty(); });
        Request request = queue.front();
        queue.pop();
        return request;
    }
};


#endif //SYNOFW_QUEUE_H
