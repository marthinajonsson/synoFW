//
// Created by mjonsson on 5/23/19.
//

#ifndef SYNOFW_QUEUE_H
#define SYNOFW_QUEUE_H


#include "Dispatcher.h"

#include <thread>
#include <queue>
#include <mutex>
#include <future>
#include <iostream>

typedef std::function<void()> Operation;

class Queue {
private:
    std::mutex qlock;
    std::queue<Operation> ops_queue;
    std::condition_variable empty;
    Dispatcher dispatcher;
public:
    Queue() = default;
    ~Queue() = default;

    void put(std::string opName) {
        std::lock_guard<std::mutex> guard(qlock);
        Operation op = dispatcher.set(opName);
        ops_queue.push(op);
        empty.notify_one();
    }

    Operation take() {
        std::unique_lock<std::mutex> lock(qlock);
        empty.wait(lock, [&]{ return !ops_queue.empty(); });

        Operation op = ops_queue.front();
        ops_queue.pop();
        return op;
    }
};


#endif //SYNOFW_QUEUE_H
