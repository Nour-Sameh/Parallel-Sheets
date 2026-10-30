#include <iostream>
#include <pthread.h>
using namespace std;

const int ThreadNo = 4;

int n;
long long mysum = 0;

pthread_mutex_t myMutex;

struct Data {
    int start;
    int end;
    long long sum;
    pthread_mutex_t* mutex;
};

void* calc(void* arg) {
    Data* d = (Data*)arg;

    d->sum = 0;

    for (int i = d->start; i <= d->end; i++) {
        d->sum += i;
    }

    pthread_mutex_lock(d->mutex);
    mysum += d->sum;
    pthread_mutex_unlock(d->mutex);

    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t mythread[ThreadNo];
    Data mydata[ThreadNo];

    pthread_mutex_init(&myMutex, NULL);

    cout << "Enter n: ";
    cin >> n;

    mysum = 0;

    for (int i = 0; i < ThreadNo; i++) {

        mydata[i].start = i * n / ThreadNo + 1;
        mydata[i].end = (i + 1) * n / ThreadNo;
        mydata[i].mutex = &myMutex;

        pthread_create(&mythread[i], NULL, calc, &mydata[i]);
    }

    for (int i = 0; i < ThreadNo; i++) {
        pthread_join(mythread[i], NULL);
    }

    cout << "Sum = " << mysum << endl;

    pthread_mutex_destroy(&myMutex);

    return 0;
}