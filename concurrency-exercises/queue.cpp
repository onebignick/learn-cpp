#include <bits/stdc++.h>
using namespace std;

counting_semaphore<0> leader(0);
counting_semaphore<0> follower(0);

void l_worker(stop_token) {
    leader.release();
    follower.acquire();
}

void f_worker(stop_token) {
    follower.release();
    leader.acquire();
}

int main() {
    
}