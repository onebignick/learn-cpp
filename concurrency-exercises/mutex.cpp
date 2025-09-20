#include <bits/stdc++.h>
using namespace std;

int counter = 0;
std::counting_semaphore<1> mtx(1);

void worker(std::stop_token) {
  for(int i = 0;i < 100;i++) {
    mtx.acquire();
    counter++;
    mtx.release();
  }
}

void solve() {
  std::jthread t1(worker), t2(worker);
}

int main() {
  solve();
  std::cout << counter << std::endl;
}
