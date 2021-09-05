#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>

namespace sign {

	//Template class implements thread safe queue storage
	template<typename T>
	class threadsafe_queue final
	{
	public:

		explicit threadsafe_queue(size_t max_length) : max_length(max_length) {}
		~threadsafe_queue()
		{
			try
			{
				release();
			}
			catch (std::exception& err)
			{
				std::cerr << err.what() << std::endl;
			}
		}

		//Take element from the queue. Blocks while the queue is empty
		std::shared_ptr<T> pop() 
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			if (queue.empty())
			{
				not_empty.wait(lock, [this]() { return !queue.empty() || done; });
				if (done)
					return std::shared_ptr<T>();
			}
			auto item = queue.front();
			queue.pop_front();

			not_full.notify_one();

			return item;
		}

		//Take element from the queue if the queue not empty. Never blocks
		std::shared_ptr<T> try_pop()
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			if (queue.empty())
			{
				return std::shared_ptr<T>();
			}
			auto item = queue.front();
			queue.pop_front();

			not_full.notify_one();

			return item;
		}

		//Put element to the queue. Blocks while the queue is full
		void push(std::shared_ptr<T> item)
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			if (queue.size() >= max_length)
			{
				not_full.wait(lock, [this]() { return queue.size() < max_length || done; });
				if (done)
					return;
			}
			queue.push_back(item);

			not_empty.notify_one();
		}

		bool empty() const 
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			return queue.empty();
		}

		size_t size() const
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			return queue.size();
		}

		void release()
		{
			done = true;
			not_empty.notify_all();
			not_full.notify_all();
		}

		//forbidden copy
		threadsafe_queue(threadsafe_queue&) = delete;
		threadsafe_queue& operator=(threadsafe_queue&) = delete;

	private:
		mutable std::mutex queue_mutex;
		std::condition_variable not_empty;
		std::condition_variable not_full;
		std::atomic_bool done{ false };
		std::deque<std::shared_ptr<T>> queue;
		size_t max_length;
	};
}