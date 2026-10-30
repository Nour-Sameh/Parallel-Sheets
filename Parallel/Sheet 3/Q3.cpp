#include <iostream>
#include <pthread.h>
using namespace std;

const int ThreadNo = 4;

int n;
int arr[100];
int maximum = LONG_MIN;

pthread_mutex_t myMutex;

void* findMax(void* arg) {
    int ThreadID = (long long)arg;

    int start = ThreadID * n / ThreadNo;
    int end = (ThreadID + 1) * n / ThreadNo;

    int max = arr[start];

    for (int i = start; i < end; i++) {
        if (arr[i] > max)
            max = arr[i];
    }

    pthread_mutex_lock(&myMutex);

    if (max > maximum)
        maximum = max;

    pthread_mutex_unlock(&myMutex);

    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t mythread[ThreadNo];

    pthread_mutex_init(&myMutex, NULL);

    cout << "Enter array size: ";
    cin >> n;

    cout << "Enter array elements:\n";

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    maximum = arr[0];

    for (int i = 0; i < ThreadNo; i++) {
        pthread_create(&mythread[i], NULL, findMax, (void*)(long long)i);
    }

    for (int i = 0; i < ThreadNo; i++) {
        pthread_join(mythread[i], NULL);
    }

    cout << "Maximum element = " << maximum << endl;

    pthread_mutex_destroy(&myMutex);

    return 0;
}