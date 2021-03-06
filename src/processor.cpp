#include <cstddef>
#include <iostream>
#include <chrono>

#include "big_file.hpp"
#include "processor.hpp"


namespace sign {

	processor::processor(const std::string& input_file, const std::string& output_file, size_t chunk_size) :
		input_file(input_file),
		output_file(output_file),
		chunk_size(chunk_size)
	{
	}

	void processor::run() 
	{
		using namespace std::chrono;
		using std::chrono::milliseconds;

		auto t0 = high_resolution_clock::now();

		auto engine = std::make_unique<signature_engine>();
		auto source_file = std::make_unique<big_file>(input_file, chunk_size);

		auto t1 = high_resolution_clock::now();

		for (size_t i = 0; ; ++i) 
		{
			auto chunk = source_file->read_next_chunk();
			if (! chunk)
				break;

			engine->submit(chunk);
		}

		auto t2 = high_resolution_clock::now();

		engine->flush(output_file);

		auto t3 = high_resolution_clock::now();

		std::cout << "Create time: " << duration_cast<milliseconds>(t1 - t0).count() << " milliseconds" << std::endl;
		std::cout << "Process time:   " << duration_cast<milliseconds>(t2 - t1).count() << " milliseconds" << std::endl;
		std::cout << "Flush time:  " << duration_cast<milliseconds>(t3 - t2).count() << " milliseconds" << std::endl;
		std::cout << "Total time:  " << duration_cast<milliseconds>(t3 - t0).count() << " milliseconds" << std::endl;
	}
}