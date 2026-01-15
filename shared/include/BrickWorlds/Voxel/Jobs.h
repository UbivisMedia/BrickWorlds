#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <atomic>

namespace BrickWorlds::Voxel {

    class JobQueue {
    public:
        using Job = std::function<void()>;

        JobQueue() = default;
        ~JobQueue() { Stop(); }

        void Start(std::size_t threads);
        void Stop();

        void Enqueue(Job job);

    private:
        void WorkerLoop();

        std::mutex mtx_;
        std::condition_variable cv_;
        std::queue<Job> q_;
        std::vector<std::thread> workers_;
        std::atomic<bool> running_{ false };
    };

} // namespace BrickWorlds::Voxel
