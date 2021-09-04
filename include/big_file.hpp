#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include <fstream>

namespace sign {

	class big_file 
	{
	public:
		struct chunk 
		{
			std::vector<std::byte> data;
			size_t number;
			bool eof = false;
		};

		big_file(const std::string& file_name, size_t chunk_size);
		std::shared_ptr<chunk> read_next_chunk();

		//forbidden copy
		big_file(big_file&) = delete;
		big_file& operator=(big_file&) = delete;

	private:
		std::string file_name;
		size_t chunk_size;
		size_t chunk_number;
		std::vector<std::byte> buffer;
		std::ifstream file;
	};
}