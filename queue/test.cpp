#include <boost/thread.hpp>
#include "queue.cpp"
#include <iostream>
#include <string>

using namespace boost;
mutex coutM;

kevinQueue::queue<int> g_queue;
int numThreads = 10;

void printHelper(std::variant<int, std::string_view> message) {
    lock_guard<mutex> lg(coutM);
    std::visit([](auto &arg) {
        if constexpr (std::is_same_v<decltype(arg), int>) 
            std::cout << arg << std::endl;
        else 
            std::cout << arg << std::endl;
    }, message);
}

void doWork() {
    g_queue.push(1);

    printHelper(g_queue.front());

    g_queue.push(2);

    printHelper(g_queue.front());
    
    g_queue.pop();

    g_queue.push(3);
    
    printHelper(g_queue.front());
}

int main(int argc, char** argv) {
	// std::cout << g_queue.front();

    if (argc > 1)
        numThreads = std::atoi(argv[1]);

    thread_group tg;
    for (int i = 0; i < numThreads; ++i) {
        tg.create_thread(doWork);
    }

    tg.join_all();
    
}
