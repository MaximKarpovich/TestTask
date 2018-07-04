#pragma once

#include <boost/thread/thread.hpp>

#include <vector>
#include <queue>
#include <condition_variable>
#include <future>

class ThreadPool {

public:
    ThreadPool(ThreadPool const&) = delete;
    ThreadPool& operator=(ThreadPool const&) = delete;

    static ThreadPool& get_instance()
    {
        static ThreadPool instance_;
        return instance_;
    }

	template<class Function, class... Args>
	void enqueue_continuous_task(Function&& func, Args&&... args);

	template<class Function, class... Args>
	void enqueue_single_task(Function&& func, Args&&... args);

	void kill_all() const;
    void wait_all() const;
    void clear_tasks() const;

private:
    explicit ThreadPool();
    virtual ~ThreadPool();    

    void add_working_thread(bool single);

private:
	mutable std::map<boost::thread::id, std::pair<bool, bool>> working_threads_map_; // first bool - means single thread or continuous
	mutable std::vector<boost::thread> working_threads_;

    mutable std::queue<std::function<void()>> tasks_;

    mutable boost::mutex tasks_queue_mutex_;

    mutable boost::condition_variable condition_;

    mutable std::function<void()> thread_function_;

    mutable bool stop_;
};

template<class Function, class... Args>
void ThreadPool::enqueue_single_task(Function&& func, Args&&... args)
{
	if (!stop_)
	{
		using return_type = typename std::result_of<Function(Args...)>::type;

		auto task = std::make_shared<std::packaged_task<return_type()>>
		(
			std::bind(std::forward<Function>(func), std::forward<Args>(args)...)
		);

		{
			boost::unique_lock<boost::mutex> lock(tasks_queue_mutex_);

			tasks_.emplace([task]() { (*task)(); });

		}

		condition_.notify_one();
	}
}

template<class Function, class... Args>
void ThreadPool::enqueue_continuous_task(Function&& func, Args&&... args)
{
	if (!stop_)
	{
		add_working_thread(false);
		enqueue_single_task(std::forward<Function>(func), std::forward<Args>(args)...);
	}
}