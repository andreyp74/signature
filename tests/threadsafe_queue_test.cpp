
#include <thread>
#include <boost/test/unit_test.hpp>
#include "threadsafe_queue.hpp"

namespace {
	void max_size_func(sign::threadsafe_queue<int>* queue)
	{
		for (int i = 0; i < 100; ++i)
		{
			queue->push(std::make_shared<int>(i));
		}
	}
}

BOOST_AUTO_TEST_CASE(max_size)
{
	sign::threadsafe_queue<int> queue(5);

	BOOST_CHECK(queue.empty());
	std::thread th(&max_size_func, &queue);

	BOOST_CHECK(queue.size() <= 5);

	for (int i = 0; i < 100; ++i)
	{
		auto val = queue.pop();
		BOOST_CHECK(queue.size() <= 5);
	}

	th.join();

	BOOST_CHECK(queue.empty());
}

namespace {
	void push_func(sign::threadsafe_queue<int>* queue)
	{
		for (int i = 0; i < 100; ++i)
		{
			queue->push(std::make_shared<int>(i));
		}
	}

	void pop_func(sign::threadsafe_queue<int>* queue)
	{
		for (int i = 0; i < 100; ++i)
		{
			auto value = queue->pop();
			BOOST_CHECK(*value == i);
		}
	}
}

BOOST_AUTO_TEST_CASE(push_pop)
{
	sign::threadsafe_queue<int> queue(10);

	std::thread push_thread(&push_func, &queue);
	std::thread pop_thread(&pop_func, &queue);

	pop_thread.join();
	push_thread.join();

	BOOST_CHECK(queue.empty()); 
}

namespace {
	void try_pop_func(sign::threadsafe_queue<int>* queue)
	{
		int prev_value = -1;
		while(true)
		{
			auto value = queue->try_pop();
			if (!value)
				continue;
			BOOST_CHECK(*value == prev_value + 1);
			prev_value = *value;
			if (*value == 99)
				break;
		}
	}
}

BOOST_AUTO_TEST_CASE(push_try_pop)
{
	sign::threadsafe_queue<int> queue(10);

	std::thread push_thread(&push_func, &queue);
	std::thread pop_thread(&try_pop_func, &queue);

	pop_thread.join();
	push_thread.join();

	BOOST_CHECK(queue.empty());
}

namespace {
	void thread_func(sign::threadsafe_queue<int>* queue)
	{
		for (int i = 0; i < 100; ++i)
		{
			queue->push(std::make_shared<int>(i));
		}
	}
}

BOOST_AUTO_TEST_CASE(release)
{
	sign::threadsafe_queue<int> queue(10);

	BOOST_CHECK(queue.empty());
	std::thread th(&thread_func, &queue);

	BOOST_CHECK(!queue.empty());

	queue.release();

	th.join();
 }