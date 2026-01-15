#include "BrickWorlds/Voxel/Jobs.h"

namespace BrickWorlds::Voxel {

    void JobQueue::Start(std::size_t threads) {
        Stop();
        running_.store(true, std::memory_order_relaxed);
        workers_.reserve(threads);
        for (std::size_t i = 0; i < threads; ++i) {
            workers_.emplace_back([this] { WorkerLoop(); });
        }
    }

    void JobQueue::Stop() {
        bool wasRunning = running_.exchange(false, std::memory_order_relaxed);
        if (!wasRunning) return;

        cv_.notify_all();
        for (auto& t : workers_) {
            if (t.joinable()) t.join();
        }
        workers_.clear();

        // Queue leeren (optional)
        std::lock_guard lk(mtx_);
        while (!q_.empty()) q_.pop();
    }

    void JobQueue::Enqueue(Job job) {
        {
            std::lock_guard lk(mtx_);
            q_.push(std::move(job));
        }
        cv_.notify_one();
    }

    void JobQueue::WorkerLoop() {
        while (running_.load(std::memory_order_relaxed)) {
            Job job;
            {
                std::unique_lock lk(mtx_);
                cv_.wait(lk, [&] {
                    return !running_.load(std::memory_order_relaxed) || !q_.empty();
                    });

                if (!running_.load(std::memory_order_relaxed)) return;
                if (q_.empty()) continue;

                job = std::move(q_.front());
                q_.pop();
            }
            job();
        }
    }

} // namespace BrickWorlds::Voxel
