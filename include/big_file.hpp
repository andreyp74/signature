#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include <fstream>

namespace sign {

	//Class implements chunk-by-chunk file reader
	class big_file 
	{
	public:
		//Structure contains file chunk data and its attributes
		struct chunk 
		{
			std::vector<std::byte> data;
			size_t number;
		};

		big_file(const std::string& file_name, size_t chunk_size);
		//Read next chunk of file
		std::shared_ptr<chunk> read_next_chunk();

		//forbidden copy
		big_file(big_file&) = delete;
		big_file& operator=(big_file&) = delete;

	private:
		std::vector<std::byte> buffer;
		std::ifstream file;
		std::string file_name;
		size_t chunk_size;
		size_t chunk_number;
	};
}