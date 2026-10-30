#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace chrono;

const int ThreadNo = 4;

int n;
int matrix[100][100];
int result[100][100];

bool isDuplicate(int i, int j) {

    for (int r = 0; r <= i; r++) {
        for (int c = 0; c < n; c++) {

            if (r == i && c >= j)
                return false;

            if (matrix[r][c] == matrix[i][j])
                return true;
        }
    }

    return false;
}

void* searchDuplicate(void* arg) {

    int ThreadID = (long long)arg;

    int start = ThreadID * n / ThreadNo;
    int end = (ThreadID + 1) * n / ThreadNo;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < n; j++) {

            if (isDuplicate(i, j))
                result[i][j] = ThreadID;
            else
                result[i][j] = matrix[i][j];

        }
    }

    pthread_exit(NULL);
}

void sequentialSearch(int seq[100][100]) {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (isDuplicate(i, j)) {
                int ThreadID = ((i + 1) * ThreadNo - 1) / n;
                seq[i][j] = ThreadID;
            }
            else
                seq[i][j] = matrix[i][j];

        }
    }
}

int main() {

    pthread_t mythread[ThreadNo];

    int seq[100][100];

    cout << "Enter matrix size: ";
    cin >> n;

    srand(time(0));

    cout << "\nOriginal Matrix:\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            matrix[i][j] = rand() % (n * n) + 1;

            cout << matrix[i][j] << "\t";
        }

        cout << endl;
    }

    // Serial Execution

    auto T1_start_time = high_resolution_clock::now();

    sequentialSearch(seq);

    auto T1_end_time = high_resolution_clock::now();

    double SerialTime =
        duration<double>(T1_end_time - T1_start_time).count();


    // Number of threads used

    int NumThread = ThreadNo;


    // Parallel Execution

    auto start_time = high_resolution_clock::now();

    for (int i = 0; i < NumThread; i++) {

        pthread_create(&mythread[i], NULL, searchDuplicate,
                       (void*)(long long)i);
    }

    for (int i = 0; i < NumThread; i++) {

        pthread_join(mythread[i], NULL);
    }

    auto end_time = high_resolution_clock::now();

    double ParallelTime =
        duration<double>(end_time - start_time).count();


    // Print Result

    cout << "\nMatrix After Searching Duplicates:\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            cout << result[i][j] << "\t";
        }

        cout << endl;
    }


    // Performance Calculations

    double Speedup = SerialTime / ParallelTime;

    double Cost = NumThread * ParallelTime;

    double Efficiency = Speedup / NumThread;


    // Print Performance

    cout << "\nSerial Time = " << SerialTime;

    cout << "\nParallel Time = " << ParallelTime;

    cout << "\nSpeedup = " << Speedup;

    cout << "\nCost = " << Cost;

    cout << "\nEfficiency = " << Efficiency;

    return 0;
}