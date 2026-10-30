#include <iostream>
#include <pthread.h>
using namespace std;

const int ThreadNo = 4;

int n;
int arr[100][2];
int result[100];

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

void* calcGCD(void* arg) {
    int ThreadID = (long long)arg;

    int start = ThreadID * n / ThreadNo;
    int end = (ThreadID + 1) * n / ThreadNo;

    for (int i = start; i < end; i++) {
        result[i] = gcd(arr[i][0], arr[i][1]);
    }

    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t mythread[ThreadNo];

    cout << "Enter number of pairs: ";
    cin >> n;

    cout << "Enter pairs of numbers:\n";

    for (int i = 0; i < n; i++) {
        cin >> arr[i][0] >> arr[i][1];
    }

    for (int i = 0; i < ThreadNo; i++) {
        pthread_create(&mythread[i], NULL, calcGCD,
                       (void*)(long long)i);
    }

    for (int i = 0; i < ThreadNo; i++) {
        pthread_join(mythread[i], NULL);
    }

    cout << "\nNumbers\t\tGCD\n";

    for (int i = 0; i < n; i++) {
        cout << "{" << arr[i][0] << ", "
             << arr[i][1] << "}\t\t"
             << result[i] << endl;
    }

    return 0;
}