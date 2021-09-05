#pragma once

#include <string>
#include "signature_engine.hpp"

namespace sign {

	//Main class of the application
	class processor {
	public:
		processor(const std::string& input_file, const std::string& output_file, size_t chunk_size);
		//Run all
		void run();

	private:
		const std::string input_file;
		const std::string output_file;
		const size_t chunk_size;
	};
}