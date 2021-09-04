#pragma once

#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <boost/uuid/detail/sha1.hpp>

namespace sign {
	class signature 
	{
	public:
		using signature_type = std::map<size_t, std::vector<unsigned int>>;

		signature();
		void add(size_t number, const std::vector<std::byte>& data);
		void flush(const std::string& file_name);

		std::vector<unsigned int> calc_hash(const std::vector<std::byte>& data);

		//forbidden copy
		signature(signature&) = delete;
		signature& operator=(signature&) = delete;

	private:
		using sha1 = boost::uuids::detail::sha1;

		std::unique_ptr<signature_type> return_signature();

	private:
		mutable std::mutex sign_mutex;
		std::unique_ptr<signature_type> sign_value;
	};
}