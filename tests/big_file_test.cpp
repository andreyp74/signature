#include <boost/test/unit_test.hpp>
#include "big_file.hpp"


 BOOST_AUTO_TEST_CASE(read)
{
	std::ofstream tmpf("./test_input_file", std::ios::binary | std::ios::trunc | std::ios::out);
	BOOST_CHECK(tmpf);

	std::vector<std::byte> part1(1024, std::byte(0));
	tmpf.write((char*)part1.data(), part1.size());

	std::vector<std::byte> part2(1024, std::byte(0x01));
	tmpf.write((char*)part2.data(), part2.size());

	std::vector<std::byte> part3(1024, std::byte(0x02));
	tmpf.write((char*)part3.data(), part3.size());

	std::vector<std::byte> part4(42, std::byte(0x03));
	tmpf.write((char*)part4.data(), part4.size());

	tmpf.close();

	sign::big_file test_file("./test_input_file", 1024);
	auto chunk1 = test_file.read_next_chunk();
	BOOST_CHECK(chunk1->data.size() == 1024);
	BOOST_CHECK(chunk1->data == part1);

	auto chunk2 = test_file.read_next_chunk();
	BOOST_CHECK(chunk2->data.size() == 1024);
	BOOST_CHECK(chunk2->data == part2);
	
	auto chunk3 = test_file.read_next_chunk();
	BOOST_CHECK(chunk3->data.size() == 1024);
	BOOST_CHECK(chunk3->data == part3);
	
	auto chunk4 = test_file.read_next_chunk();
	BOOST_CHECK(chunk4->data.size() == 1024); 

	std::vector<std::byte> part4_filled0(1024);
	std::copy(part4.begin(), part4.end(), part4_filled0.begin());
	BOOST_CHECK(chunk4->data == part4_filled0);
	
	auto chunk5 = test_file.read_next_chunk();
	BOOST_CHECK(!chunk5);
}