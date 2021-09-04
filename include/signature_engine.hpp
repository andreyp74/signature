#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <map>

#include "threadsafe_queue.hpp"
#include "big_file.hpp"
#include "signature.hpp"


namespace sign {

	//Class implements thread pool for processing file chunks
	class signature_engine final {
	public:

		signature_engine();
		~signature_engine();

		//Submit chunk for processing
		void submit(std::shared_ptr<big_file::chunk> chunk);
		//Flush result to file
		void flush(const std::string& file_name);

		//forbidden copy
		signature_engine(signature_engine&) = delete;
		signature_engine& operator=(signature_engine&) = delete;

	private:

		void run();
		void stop();

	private:
		std::vector<std::thread> thread_pool;
		std::unique_ptr<threadsafe_queue<big_file::chunk>> queue;
		std::unique_ptr<signature> sign;
		std::atomic_bool done{ false };
	};
}