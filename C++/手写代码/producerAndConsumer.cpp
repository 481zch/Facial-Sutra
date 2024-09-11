#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> q;
const unsigned int maxApples = 10;
bool done = false;

void producer()
{
	int data = 0;
	while (!done) {
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [] {return q.size() < maxApples; });
		q.push(++data);
		std::cout << "producer " << std::this_thread::get_id() << " produced: " << data << std::endl;
		lock.unlock();
		cv.notify_all();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void consumer()
{
	while (!done) {
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [] {return !q.empty(); });

		int data = q.front();
		q.pop();
		std::cout << "consumer " << std::this_thread::get_id() << " consumed: " << data << std::endl;

		lock.unlock();
		cv.notify_all();
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
}


int main()
{
	std::thread t1(producer);

	std::thread t2(consumer);

	std::this_thread::sleep_for(std::chrono::seconds(5));
	done = true;

	t1.join();
	t2.join();

	return 0;
}