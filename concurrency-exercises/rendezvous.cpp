#include <bits/stdc++.h>

std::mutex mtx;
std::condition_variable cv;

int arrived = 0;

void mutex_worker() {
  std::cout << 1 << std::endl;

  std::unique_lock<std::mutex> lock(mtx);
  arrived++;
  if (arrived < 2) {
    cv.wait(lock, []{ return arrived == 2; }); // this unlocks the mutex and reloacks when it wakes
  } else {
    cv.notify_all();
  }

  std::cout << 2 << std::endl;
}

std::counting_semaphore<1> sem1(0);
std::counting_semaphore<1> sem2(0);

void sem_worker(int id) {
  std::cout << 1 << std::endl;
  if (id == 1) {
    sem1.release();
    sem2.acquire();
  } else {
    sem2.release();
    sem1.acquire();
  }

  std::cout << 2 << std::endl;
}

int main() {
  // std::thread t1(mutex_worker), t2(mutex_worker);
  std::thread t1(sem_worker, 1), t2(sem_worker, 2);
  t1.join();
  t2.join();
}
