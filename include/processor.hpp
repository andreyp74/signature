#pragma once

#include <string>
#include "signature_engine.hpp"

namespace sign {

	class processor {
	public:
		processor(const std::string& input_file, const std::string& output_file, size_t chunk_size);
		void run();

	private:
		std::string input_file;
		std::string output_file;
		size_t chunk_size;
	};
}