#include <iomanip>
#include <sstream>
#include <fstream>

#include <boost/test/unit_test.hpp>
#include "signature_engine.hpp"
#include "big_file.hpp"

namespace {
	std::string digest_to_string(const std::vector<unsigned int>& digest)
	{
		std::ostringstream buf;
		for (int i = 0; i < 5; ++i)
			buf << std::hex << std::setfill('0') << std::setw(8) << digest[i];
		return buf.str();
	}
}

BOOST_AUTO_TEST_CASE(engine_flush)
{
	std::string s0 = "";
	std::string s1 = "The quick brown fox jumps over the lazy dog";
	std::string s2 = "The quick brown fox jumps over the lazy cog";
	std::vector<std::byte> data0((const std::byte*)s0.c_str(), (const std::byte*)(s0.c_str() + s0.size()));
	std::vector<std::byte> data1((const std::byte*)s1.c_str(), (const std::byte*)(s1.c_str() + s1.size()));
	std::vector<std::byte> data2((const std::byte*)s2.c_str(), (const std::byte*)(s2.c_str() + s2.size()));

	sign::signature_engine engine;
	auto chunk0 = std::make_shared<sign::big_file::chunk>(sign::big_file::chunk{ data0, 0, false });
	engine.submit(chunk0);
	auto chunk1 = std::make_shared<sign::big_file::chunk>(sign::big_file::chunk{ data1, 1, false });
	engine.submit(chunk1);
	auto chunk2 = std::make_shared<sign::big_file::chunk>(sign::big_file::chunk{ data2, 2, false });
	engine.submit(chunk2);

	engine.flush("./test_engine_result");

	std::ifstream fs("./test_engine_result", std::ios::binary | std::ios::in);
	BOOST_CHECK(fs);

	std::vector<unsigned int> buffer0(5);
	fs.read((char*)buffer0.data(), sizeof(unsigned int) * 5);
	auto hash_str0 = digest_to_string(buffer0);
	BOOST_CHECK(hash_str0 == "da39a3ee5e6b4b0d3255bfef95601890afd80709");

	std::vector<unsigned int> buffer1(5);
	fs.read((char*)buffer1.data(), sizeof(unsigned int) * 5);
	auto hash_str1 = digest_to_string(buffer1);
	BOOST_CHECK(hash_str1 == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");

	std::vector<unsigned int> buffer2(5);
	fs.read((char*)buffer2.data(), sizeof(unsigned int) * 5);
	auto hash_str2 = digest_to_string(buffer2);
	BOOST_CHECK(hash_str2 == "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3");

	auto next = fs.peek();
	BOOST_CHECK(next == std::ifstream::traits_type::eof());

	fs.close();
 }