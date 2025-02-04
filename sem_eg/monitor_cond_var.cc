// Copyright CSCE311 SP25
//

#include <pthread.h>
#include <unistd.h>

#include <iostream>

// Monitor class with mutex and condition variable
class Monitor {
public:
    Monitor() : ready(0) {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
    }

    ~Monitor() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    void waitForSignal() {
        pthread_mutex_lock(&mutex);  // Lock monitor

        while (ready == 0) {
            std::cout << "Thread " << pthread_self() << " waiting...\n";
            pthread_cond_wait(&cond, &mutex);  // Wait for signal
        }

        std::cout << "Thread " << pthread_self() << " received signal and proceeds.\n";

        pthread_mutex_unlock(&mutex);  // Unlock monitor
    }

    void sendSignal() {
        sleep(2);  // Simulate work

        pthread_mutex_lock(&mutex);  // Lock monitor
        ready = 1;  // Update condition
        std::cout << "Thread " << pthread_self() << " signaling waiting thread.\n";

        pthread_cond_signal(&cond);  // Wake up waiting thread
        pthread_mutex_unlock(&mutex);  // Unlock monitor
    }

private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int ready;  // Shared condition flag
};

// Shared monitor instance
Monitor resource;

// Worker function that waits for the condition to be met
void* waiting_thread(void*) {
    resource.waitForSignal();
    return nullptr;
}

// Worker function that signals the waiting thread
void* signaling_thread(void*) {
    resource.sendSignal();
    return nullptr;
}

int main() {
    pthread_t t1, t2;

    // Create a waiting thread and a signaling thread
    pthread_create(&t1, nullptr, waiting_thread, nullptr);
    pthread_create(&t2, nullptr, signaling_thread, nullptr);

    // Wait for threads to finish
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    return 0;
}
