#include <iostream>
#include <pthread.h>
using namespace std;

const int ThreadNo = 4;

int rows, cols;

int matrix[100][100];
int vec[100];
int result[100];

void* multiply(void* arg) {
    int ThreadID = (long long)arg;

    int start = ThreadID * rows / ThreadNo;
    int end = (ThreadID + 1) * rows / ThreadNo;

    for (int i = start; i < end; i++) {
        result[i] = 0;

        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }

    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t mythread[ThreadNo];

    cout << "Enter rows and columns: ";
    cin >> rows >> cols;

    cout << "Enter Matrix:\n";

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> matrix[i][j];
        }
    }

    cout << "Enter Vector:\n";

    for (int i = 0; i < cols; i++) {
        cin >> vec[i];
    }

    for (int i = 0; i < ThreadNo; i++) {
        pthread_create(&mythread[i], NULL, multiply, (void*)(long long)i);
    }

    for (int i = 0; i < ThreadNo; i++) {
        pthread_join(mythread[i], NULL);
    }

    cout << "Result Vector:\n";

    for (int i = 0; i < rows; i++) {
        cout << result[i] << " ";
    }

    cout << endl;

    return 0;
}