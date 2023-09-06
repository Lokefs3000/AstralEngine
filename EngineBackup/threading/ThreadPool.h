#pragma once

#include <mutex>
#include <vector>
#include <queue>
#include <functional>

#include "Variables.h"

//https://stackoverflow.com/questions/15752659/thread-pooling-in-c11

class EXPORT ThreadPool {
private:
    void ThreadLoop();

    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::condition_variable pool_finished;
    std::vector<std::thread> threads;
    std::queue<std::function<void(void)>> jobs;
public:
    void Start(uint32_t maxThreads = std::thread::hardware_concurrency());
    void QueueJob(const std::function<void(void)>& job);
    void Stop();

    bool busy();
    void wait();
};