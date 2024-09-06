#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

template <typename Event> class EventQueue
{
public:
    EventQueue(std::function<void(const std::vector<Event> &)> processFunc)
        : processFunc_(processFunc), stopFlag_(false)
    {
        workerThread_ = std::thread(&EventQueue::processLoop, this);
    }

    ~EventQueue()
    {
        // Stop the worker thread and clean up
        stopFlag_ = true;
        condVar_.notify_all();
        if (workerThread_.joinable())
        {
            workerThread_.join();
        }
    }

    // Add an event to the queue
    void addEvent(const Event &event)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(event);
        }
        condVar_.notify_one();
    }

private:
    void processLoop()
    {
        while (!stopFlag_)
        {
            std::unique_lock<std::mutex> lock(mutex_);

            // Wait for an event or stop signal
            condVar_.wait(lock, [this]() { return !queue_.empty() || stopFlag_; });

            if (stopFlag_ && queue_.empty())
            {
                break;
            }

            std::vector<Event> events;
            while (!queue_.empty())
            {
                events.push_back(queue_.front());
                queue_.pop();
            }

            lock.unlock(); // Unlock while processing events
            processFunc_(events);
        }
    }

    std::queue<Event> queue_;
    std::function<void(const std::vector<Event> &)> processFunc_;
    std::mutex mutex_;
    std::condition_variable condVar_;
    std::atomic<bool> stopFlag_;
    std::thread workerThread_;
};
