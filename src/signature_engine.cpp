
#include <iostream>
#include <chrono>

#include "signature_engine.hpp"


namespace sign {

	const size_t QUEUE_RATE = 16;

	signature_engine::signature_engine() : sign(std::make_unique<signature>())
	{
		auto num_threads = std::thread::hardware_concurrency() - 1;
		queue.reset(std::make_unique<threadsafe_queue<big_file::chunk>>(QUEUE_RATE * num_threads).release());

		for (size_t i = 0; i < num_threads; ++i)
		{
			thread_pool.emplace_back(&signature_engine::run, this);
		}
	}

	signature_engine::~signature_engine()
	{
		try
		{
			stop();
		}
		catch (std::exception& err)
		{
			std::cerr << err.what() << std::endl;
		}
	}

	void signature_engine::submit(std::shared_ptr<big_file::chunk> chunk)
	{
		queue->push(chunk);
	}

	void signature_engine::stop()
	{
		done = true;
		for (auto& thread : thread_pool)
		{
			if (thread.joinable())
				thread.join();
		}
	}

	//Thread function
	void signature_engine::run()
	{
		try
		{
			while (!done)
			{
				auto chunk = queue->try_pop();
				if (chunk)
				{
					sign->append(chunk->number, chunk->data);
				}
			}

			while (!queue->empty())
			{
				auto chunk = queue->try_pop();
				if (chunk)
				{
					sign->append(chunk->number, chunk->data);
				}
			}
		}
		catch (std::exception& err)
		{
			std::cerr << "signature_engine: error occurred: " << err.what() << std::endl;
		}
	}

	void signature_engine::flush(const std::string& file_name)
	{
		stop();
		sign->flush(file_name);
	}
}