
#include "big_file.hpp"

#include <iostream>

namespace sign {

	big_file::big_file(const std::string& file_name, size_t chunk_size)
		: file_name(file_name), chunk_size(chunk_size), chunk_number(0) 
	{
		file.open(file_name, std::ios::binary | std::ios::in);
		if (!file) {
			std::cerr << "Couldn't open file: " << file_name << std::endl;
		}
		//TODO
		buffer.resize(chunk_size, std::byte(0));
	}

	std::shared_ptr<big_file::chunk> big_file::read_next_chunk() 
	{
		auto file_chunk = std::make_shared<chunk>();

		if (file) 
		{
			memset(buffer.data(), 0, buffer.size());
			file.read((char*)&buffer[0], chunk_size);
			file_chunk->number = chunk_number;
			// this is a copy!!!
			file_chunk->data.assign(buffer.begin(), buffer.end());

			++chunk_number;
		}
		else 
		{
			file_chunk->eof = true;
		}

		return file_chunk;
	}
}