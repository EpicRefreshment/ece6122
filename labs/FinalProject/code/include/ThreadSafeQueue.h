/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/05/2025
Date Last Modified: 11/05/2025

Description:

Final Project

A simple thread-safe queue for passing events between threads.
*/

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue
{
public:
    void push(T value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::move(value));
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            return false;
        value = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};