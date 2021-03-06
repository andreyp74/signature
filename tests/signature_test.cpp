#include <iomanip>
#include <sstream>
#include <fstream>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "signature.hpp"

namespace {
	std::string digest_to_string(const std::vector<unsigned int>& digest)
	{
		std::ostringstream buf;
		for (int i = 0; i < 5; ++i)
			buf << std::hex << std::setfill('0') << std::setw(8) << digest[i];
		return buf.str();
	}
}

BOOST_AUTO_TEST_CASE(empty_hash)
{
	sign::signature sign;
	std::vector<std::byte> data;
	auto hash = sign.calc_hash(data);
	auto empty = digest_to_string(hash);
	BOOST_CHECK(empty == "da39a3ee5e6b4b0d3255bfef95601890afd80709");
}

BOOST_AUTO_TEST_CASE(calc_hash)
{
	sign::signature sign;
	std::string s0 = "";
	std::string s1 = "The quick brown fox jumps over the lazy dog";
	std::string s2 = "The quick brown fox jumps over the lazy cog";
	std::vector<std::byte> data0((const std::byte*)s0.c_str(), (const std::byte*)(s0.c_str() + s0.size()));
	std::vector<std::byte> data1((const std::byte*)s1.c_str(), (const std::byte*)(s1.c_str() + s1.size()));
	std::vector<std::byte> data2((const std::byte*)s2.c_str(), (const std::byte*)(s2.c_str() + s2.size()));

	auto hash0 = sign.calc_hash(data0);
	auto hash_str0 = digest_to_string(hash0);
	BOOST_CHECK(hash_str0 == "da39a3ee5e6b4b0d3255bfef95601890afd80709");

	auto hash1 = sign.calc_hash(data1);
	auto hash_str1 = digest_to_string(hash1);
	BOOST_CHECK(hash_str1 == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");

	auto hash2 = sign.calc_hash(data2);
	auto hash_str2 = digest_to_string(hash2);
	BOOST_CHECK(hash_str2 == "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3");
}

BOOST_AUTO_TEST_CASE(reentrant_calc_hash)
{
	sign::signature sign;
	std::string s = "The quick brown fox jumps over the lazy dog";
	std::vector<std::byte> data((const std::byte*)s.c_str(), (const std::byte*)(s.c_str() + s.size()));

	auto hash1 = sign.calc_hash(data);
	auto hash_str1 = digest_to_string(hash1);
	BOOST_CHECK(hash_str1 == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");

	auto hash2 = sign.calc_hash(data);
	auto hash_str2 = digest_to_string(hash2);
	BOOST_CHECK(hash_str2 == hash_str1);
}

BOOST_AUTO_TEST_CASE(empty_signature)
{
	sign::signature sign;
	sign.flush("./empty_signature");

	std::ifstream fs("./empty_signature", std::ios::binary | std::ios::in);
	BOOST_CHECK(fs);
	auto next = fs.peek();
	BOOST_CHECK(next == std::ifstream::traits_type::eof());
	fs.close();
}

BOOST_AUTO_TEST_CASE(flush)
{
	sign::signature sign;
	std::string s0 = "";
	std::string s1 = "The quick brown fox jumps over the lazy dog";
	std::string s2 = "The quick brown fox jumps over the lazy cog";
	std::vector<std::byte> data0((const std::byte*)s0.c_str(), (const std::byte*)(s0.c_str() + s0.size()));
	std::vector<std::byte> data1((const std::byte*)s1.c_str(), (const std::byte*)(s1.c_str() + s1.size()));
	std::vector<std::byte> data2((const std::byte*)s2.c_str(), (const std::byte*)(s2.c_str() + s2.size()));
	sign.append(0, data0);
	sign.append(1, data1);
	sign.append(2, data2);

	sign.flush("./test_sign");

	std::ifstream fs("./test_sign", std::ios::binary | std::ios::in);
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
