#include <iostream>
#include <pthread.h>
using namespace std;

const int ThreadNo = 2;

int n;
int arr[100];

int EvenArr[100];
int OddArr[100];

int evenIndex = 0;
int oddIndex = 0;

pthread_mutex_t myMutex;

void* countDigits(void* arg) {
    int ThreadID = (long long)arg;

    int start = ThreadID * n / ThreadNo;
    int end = (ThreadID + 1) * n / ThreadNo;

    int even[100], odd[100];
    int ec = 0, oc = 0;

    for (int i = start; i < end; i++) {
        int num = arr[i];

        if (num == 0) {
            even[ec++] = 0;
        }

        while (num > 0) {
            int digit = num % 10;

            if (digit % 2 == 0)
                even[ec++] = digit;
            else
                odd[oc++] = digit;

            num /= 10;
        }
    }

    pthread_mutex_lock(&myMutex);

    for (int i = 0; i < ec; i++)
        EvenArr[evenIndex++] = even[i];

    for (int i = 0; i < oc; i++)
        OddArr[oddIndex++] = odd[i];

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

    for (int i = 0; i < n; i++)
        cin >> arr[i];

    for (int i = 0; i < ThreadNo; i++) {
        pthread_create(&mythread[i], NULL, countDigits,(void*)(long long)i);
    }

    for (int i = 0; i < ThreadNo; i++)
        pthread_join(mythread[i], NULL);

    int evenSum = 0;
    int oddSum = 0;

    for (int i = 0; i < evenIndex; i++)
        evenSum += EvenArr[i];

    for (int i = 0; i < oddIndex; i++)
        oddSum += OddArr[i];

    EvenArr[evenIndex] = evenSum;
    OddArr[oddIndex] = oddSum;

    cout << "EvenArr: ";

    for (int i = 0; i <= evenIndex; i++)
        cout << EvenArr[i] << " ";

    cout << "\nOddArr: ";

    for (int i = 0; i <= oddIndex; i++)
        cout << OddArr[i] << " ";

    cout << endl;

    pthread_mutex_destroy(&myMutex);

    return 0;
}