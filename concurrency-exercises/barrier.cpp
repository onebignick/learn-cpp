#include <bits/stdc++.h>

const int MAX_THREADS = 10;
std::atomic<int> waiting = 0;
std::atomic<int> running = 0;
std::counting_semaphore<1> sem(0);

void worker(std::stop_token, int id) {
    waiting.fetch_add(1);
    std::cout << "Current threads waiting: " << waiting << std::endl;
    if (waiting+1 == MAX_THREADS) {
        for(int i = 0;i < MAX_THREADS; i++) {
            sem.release();
        }
    }

    sem.acquire();
    sem.release();
    running.fetch_add(1);
    std::cout << "Current threads running: " << running << std::endl;
}

int main() {
    std::vector<std::jthread> threads(MAX_THREADS);
    for(int i = 0;i < MAX_THREADS; i++) {
        threads[i] = std::jthread(worker, i);
    }
}