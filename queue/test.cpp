#include <boost/thread>
#include "queue.cpp"
#include <iostream>

kevin_queue::queue<int> g_queue;


void testing() {
    g_queue
}

int main() {
	kevin_queue::queue<int> q;
	const kevin_queue::queue<int>& q2 = q;
	std::cout << q2.front();
	for (int i = 0; i < 10; ++i) q.push(i);
	std::cout << q.capacity() << " " << q.size() << std::endl;
	for (int i = 0; i < 10; ++i) {
		std::cout << q.front();
		q.pop();
	} 
}
