#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool
{
public:
    // Constructor: Initializes the thread pool with a given number of threads
    ThreadPool(size_t threadCount);

    // Destructor: Waits for all threads to complete and stops the thread pool
    ~ThreadPool();

    // Adds a task to the queue
    template<typename Task>
    void enqueueTask(Task task);

private:
    // Function executed by worker threads
    void workerThread();

    // Worker threads that process tasks from the queue
    std::vector<std::thread> workers;
    // Task queue to hold the tasks
    std::queue<std::function<void()>> taskQueue;

    // Synchronization mechanisms
    std::mutex queueMutex; // Protects access to the queue
    std::condition_variable condition; // Used for thread synchronization
    std::atomic<bool> stopFlag; // Signals threads to stop execution
};
