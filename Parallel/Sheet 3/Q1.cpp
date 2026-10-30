#include <iostream>
#include <pthread.h>
using namespace std;

const int ThreadNo = 2;

int n;
int arr[100];

int odd = 0;
int even = 0;

pthread_mutex_t myMutex;

void* countNumbers(void* arg) {
    int ThreadID = (long long)arg;

    int start = ThreadID * n / ThreadNo;
    int end = (ThreadID + 1) * n / ThreadNo;

    int oddCount = 0;
    int evenCount = 0;

    for (int i = start; i < end; i++) {
        if (arr[i] % 2 == 0)
            evenCount++;
        else
            oddCount++;
    }

    pthread_mutex_lock(&myMutex);

    odd += oddCount;
    even += evenCount;

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

    for (int i = 0; i < ThreadNo; i++) {
        pthread_create(&mythread[i], NULL, countNumbers, (void*)(long long)i);
    }

    for (int i = 0; i < ThreadNo; i++) {
        pthread_join(mythread[i], NULL);
    }

    cout << "Odd numbers = " << odd << endl;
    cout << "Even numbers = " << even << endl;

    pthread_mutex_destroy(&myMutex);

    return 0;
}