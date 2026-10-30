#include <iostream>
#include <pthread.h>
#include <ctime>
using namespace std;

const int ThreadNo = 4;

int finalEnd;
int mysum;
pthread_mutex_t myMutex;

bool isPrime(int n) {
    if (n < 2)
        return false;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }

    return true;
}

int countPrime(int start, int end) {
    int count = 0;

    for (int i = start; i <= end; i++) {
        if (isPrime(i))
            count++;
    }

    return count;
}

void* calc(void* arg) {
    int ThreadID = (long long)arg;

    int start = ThreadID * finalEnd / ThreadNo + 1;
    int end = (ThreadID + 1) * finalEnd / ThreadNo;

    int primecount = countPrime(start, end);

    pthread_mutex_lock(&myMutex);
    mysum += primecount;
    pthread_mutex_unlock(&myMutex);

    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t mythread[ThreadNo];

    pthread_mutex_init(&myMutex, NULL);

    int values[] = {
        10, 100, 1000, 10000, 100000, 1000000
    };

    cout << "x\t\tpi(x)\t\twTime" << endl;

    for (int k = 0; k < 6; k++) {
        finalEnd = values[k];
        mysum = 0;

        clock_t startclock = clock();

        for (int i = 0; i < ThreadNo; i++) {
            pthread_create(
                &mythread[i],
                NULL,
                calc,
                (void*)(long long)i
            );
        }

        for (int i = 0; i < ThreadNo; i++) {
            pthread_join(mythread[i], NULL);
        }

        double duration =
            (clock() - startclock) / (double)CLOCKS_PER_SEC;

        cout << values[k] << "\t\t"
             << mysum << "\t\t"
             << duration << " sec" << endl;
    }

    pthread_mutex_destroy(&myMutex);

    return 0;
}
