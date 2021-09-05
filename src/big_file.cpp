
#include "big_file.hpp"

#include <iostream>

namespace sign {

	big_file::big_file(const std::string& file_name, size_t chunk_size)
		: file_name(file_name), chunk_size(chunk_size), chunk_number(0) 
	{
		file.open(file_name, std::ios::binary | std::ios::in);
		if (! file) 
		{
			throw std::runtime_error("Couldn't open file: " + file_name);
		}
	}

	std::shared_ptr<big_file::chunk> big_file::read_next_chunk() 
	{
		if (file)
		{
			buffer.resize(chunk_size, std::byte(0));
			file.read((char*)buffer.data(), chunk_size);

			auto file_chunk = std::make_shared<chunk>();
			file_chunk->data.swap(buffer);
			file_chunk->number = chunk_number;

			++chunk_number;
			return file_chunk;
		}
		return nullptr;
	}
}