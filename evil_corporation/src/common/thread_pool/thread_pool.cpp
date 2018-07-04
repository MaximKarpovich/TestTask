#include "thread_pool.h"

#include <utility>

/*the constructor just launches some amount of workers*/
ThreadPool::ThreadPool() : stop_(false)
{
	uint8_t max_thread_queue_number = boost::thread::hardware_concurrency();

    if (max_thread_queue_number == 0)
		max_thread_queue_number = 4;

    thread_function_ = [this]
    {
		this->working_threads_map_[boost::this_thread::get_id()] = std::make_pair<bool, bool>(true, true);
		try {
			for (;;)
			{
				std::function<void()> task;

				{
					boost::unique_lock<boost::mutex> lock(this->tasks_queue_mutex_);
					this->condition_.wait(lock, [this] { return this->stop_ || !this->tasks_.empty(); });
					if (this->stop_ && this->tasks_.empty())
						return;

					task = std::move(this->tasks_.front());
					this->tasks_.pop();
				}

				this->working_threads_map_[boost::this_thread::get_id()].second = true;

				task();

				this->working_threads_map_[boost::this_thread::get_id()].second = false;
			}
		}
		catch (boost::thread_interrupted&)
		{

		}
    };

    for (uint8_t i = 0; i < max_thread_queue_number; ++i)
        add_working_thread(true);
}

/* The destructor joins all threads*/
ThreadPool::~ThreadPool()
{
	wait_all();
}

void ThreadPool::add_working_thread(bool single)
{
	//join finished threads
	for (int i = 0; i < working_threads_.size(); i++)
	{
		if (working_threads_map_[working_threads_[i].get_id()].first == false) //means continuous thread
		{
			if (working_threads_map_[working_threads_[i].get_id()].second == false)
				working_threads_[i].join();
		}
	}

	boost::thread new_thread(thread_function_);

	working_threads_map_[new_thread.get_id()] = std::make_pair<bool, bool>(single ? true : false, false);

	working_threads_.emplace_back(std::move(new_thread));
}

void ThreadPool::wait_all() const
{
	{
		boost::unique_lock<boost::mutex> lock(tasks_queue_mutex_);
		stop_ = true;
	}

    condition_.notify_all();

	for (boost::thread &worker : working_threads_) {
        if (worker.joinable())
            worker.join();
    }
}

void ThreadPool::clear_tasks() const
{
    std::queue<std::function<void()>> empty;

	boost::unique_lock<boost::mutex> lock(tasks_queue_mutex_);

    std::swap(tasks_, empty);
}

void ThreadPool::kill_all() const
{
	{
		boost::unique_lock<boost::mutex> lock(tasks_queue_mutex_);
		stop_ = true;
	}

	condition_.notify_all();

	for (boost::thread &worker : working_threads_) {
		worker.interrupt();
		if (worker.joinable())
		{
			worker.join();
		}
	}
}