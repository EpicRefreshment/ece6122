#include <ThreadPool.h>

// Constructor: Initializes the thread pool with a given number of threads
ThreadPool::ThreadPool(size_t threadCount) 
    : stopFlag(false)
{
    for (size_t i = 0; i < threadCount; i++)
    {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

// Destructor: Waits for all threads to complete and stops the thread pool
ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stopFlag = true;
    }
    condition.notify_all(); // Wake up all threads to finish
    for (std::thread& worker : workers)
    {
        if (worker.joinable())
            worker.join(); // Wait for threads to finish execution
    }
}

// Function executed by worker threads
void ThreadPool::workerThread()
{
    while (true)
    {
        // https://www.geeksforgeeks.org/std-function-in-cpp/
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock,
                [this]() {
                    return stopFlag || !taskQueue.empty(); // Wait for tasks or stop signal
                });

            if (stopFlag && taskQueue.empty())
                return; // Exit the thread

            task = std::move(taskQueue.front());
            taskQueue.pop();
        }
        task(); // Execute the task
    }
}
