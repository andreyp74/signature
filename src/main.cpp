#include <iostream>
#include <string>
#include "processor.hpp"


int main(int argc, char *argv[]) 
{
	if (argc < 3)
	{
		std::cerr << "Usage: Signature <input_file> <output_file> [chunk_size_in_mbytes]\n";
		return 1;
	}

	size_t chunk_size = 1024 * 1024; // default chunk_size == 1Mb
	if (argc == 4)
	{
		chunk_size = std::stoul(argv[3]) * 1024 * 1024;
	}

	try
	{
		sign::processor proc(argv[1], argv[2], chunk_size);
		proc.run();
	}
	catch (std::exception& err) 
	{
		std::cerr << "Signature: error occurred: " << err.what() << std::endl;
	}

    return 0;
}