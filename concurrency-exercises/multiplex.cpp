#include <bits/stdc++.h>

const int MAX_AMOUNT = 4;
int total_threads_inside = 0;
std::counting_semaphore<MAX_AMOUNT> bouncer(MAX_AMOUNT);

void worker(std::stop_token, int id) {
    bouncer.acquire();
    total_threads_inside++;
    std::cout << "Thread_count: " << total_threads_inside << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    total_threads_inside--;
    bouncer.release();
}

int main() {
    std::vector<std::jthread> threads(10);   
    for(int i = 0;i < 10; i++) {
        threads[i] = std::jthread(worker, i);
    }
}