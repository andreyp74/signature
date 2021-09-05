#include <fstream>
#include <iostream>
#include <cstdio>
#include "signature.hpp"

namespace sign {

	signature::signature() : sign_value(std::make_unique<signature_type>()) 
	{}

	std::vector<unsigned int> signature::calc_hash(const std::vector<std::byte>& data)
	{
		unsigned int digest[5];
		sha1 hash_function;
		hash_function.process_bytes(data.data(), data.size());
		hash_function.get_digest(digest);

		return std::vector<unsigned int>(digest, digest + sizeof(digest) / sizeof(unsigned int));
	}

	void signature::append(size_t number, const std::vector<std::byte>& data)
	{
		auto hash = calc_hash(data);

		std::lock_guard<std::mutex> lock(sign_mutex);
		sign_value->emplace(number, std::move(hash));
	}

	std::unique_ptr<signature::signature_type> signature::return_signature() 
	{
		std::lock_guard<std::mutex> lock(sign_mutex);
		return std::move(sign_value);
	}

	void signature::flush(const std::string& file_name)
	{
		std::string tmp_file_name = std::tmpnam(nullptr);
		std::ofstream result(tmp_file_name, std::ios::binary | std::ios::out | std::ios::trunc);
		if (!result)
		{
			std::cerr << "Couldn't create signature file: " << tmp_file_name << std::endl;
			return;
		}

		auto sign_tmp = return_signature();
		for (auto& sign : *sign_tmp)
		{
			result.write((const char*)&sign.second[0], sign.second.size() * sizeof(sign.second[0]));
		}
		result.close();

		std::remove(file_name.c_str());
		//rename temporary file to the target
		if (std::rename(tmp_file_name.c_str(), file_name.c_str()))
			std::cerr << "Error during creation of the signature file: " << file_name << std::endl;
		else
			std::cout << "File with signature " << file_name << " was written successfully" << std::endl;
	}
}