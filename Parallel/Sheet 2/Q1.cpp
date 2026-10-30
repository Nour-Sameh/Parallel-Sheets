#include <iostream>
#include <pthread.h>
using namespace std;

const int ThreadNo = 4;

int n;
long long mysum;
pthread_mutex_t myMutex;

void* calc(void* arg) {
    int ThreadID = (long long)arg;

    int start = ThreadID * n / ThreadNo + 1;
    int end = (ThreadID + 1) * n / ThreadNo;

    long long sum = 0;

    for (int i = start; i <= end; i++) {
        sum += i;
    }

    pthread_mutex_lock(&myMutex);
    mysum += sum;
    pthread_mutex_unlock(&myMutex);

    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t mythread[ThreadNo];

    pthread_mutex_init(&myMutex, NULL);

    cout << "Enter n: ";
    cin >> n;

    mysum = 0;

    for (int i = 0; i < ThreadNo; i++) {
        pthread_create(&mythread[i], NULL, calc, (void*)(long long)i);
    }

    for (int i = 0; i < ThreadNo; i++) {
        pthread_join(mythread[i], NULL);
    }

    cout << "Sum = " << mysum << endl;

    pthread_mutex_destroy(&myMutex);

    return 0;
}