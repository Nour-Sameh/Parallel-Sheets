
#include <bits/stdc++.h>
#include <pthread.h>
#include <stdint.h>
using namespace std;

int n;
const int th_num = 2;

long long arr[100];
long long EvenArr[101], OddArr[101];

long long even = 0, odd = 0;

pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;

void* calc(void* args)
{
    int id = (intptr_t)args;

    int st = id * n / th_num;
    int ed = (id + 1) * n / th_num;

    long long localEven = 0;
    long long localOdd = 0;

    for (int i = st; i < ed; i++)
    {
        long long x = arr[i];

        int e = 0, o = 0;

        do
        {
            int digit = x % 10;

            if (digit % 2 == 0)
            {
                e++;
                localEven += digit;
            }
            else
            {
                o++;
                localOdd += digit;
            }

            x /= 10;

        } while (x > 0);

        EvenArr[i] = e;
        OddArr[i] = o;
    }

    pthread_mutex_lock(&mux);

    even += localEven;
    odd += localOdd;

    pthread_mutex_unlock(&mux);

    return NULL;
}

int main()
{
    cout << "Enter array size: ";
    cin >> n;

    cout << "Enter array elements:\n";

    for (int i = 0; i < n; i++)
        cin >> arr[i];

    pthread_t th[th_num];

    for (int i = 0; i < th_num; i++)
        pthread_create(&th[i], NULL, calc, (void*)(intptr_t)i);

    for (int i = 0; i < th_num; i++)
        pthread_join(th[i], NULL);

    EvenArr[n] = even;
    OddArr[n] = odd;

    cout << "\nEvenArr: ";
    for (int i = 0; i <= n; i++)
        cout << EvenArr[i] << " ";

    cout << "\nOddArr: ";
    for (int i = 0; i <= n; i++)
        cout << OddArr[i] << " ";

    return 0;
}
