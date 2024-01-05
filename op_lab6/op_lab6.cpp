#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cmath>

using namespace std;

struct workerInfo {
    double* A;
    int n;
    int kolvo;
};

condition_variable cvSort;
condition_variable cvKolvo;
mutex mtx1;
mutex mtx2;
mutex mtx3;

void Work(workerInfo& info) {
    int interval;
    cout << "Enter interval(ms): ";
    cin >> interval;
    int k = 0;
    for (int i = 0; i < info.n; ++i) {
        if (info.A[i] > 0)
        {
            double e = info.A[i];
            info.A[i] = sqrt(e);
            if (info.A[k] != info.A[i]) {
                swap(info.A[k], info.A[i]);
            }
            k++;
        }
        this_thread::sleep_for(chrono::milliseconds(interval));
    }
    cvSort.notify_one();
}

void Count(workerInfo& info) {
    mtx3.lock();
    info.kolvo = 0;
    for (int i = 0; i < info.n; ++i) {
        if (info.A[i] == round(info.A[i]))
            info.kolvo += 1;
    }
    mtx3.unlock();
    cvKolvo.notify_one();
}

int main()
{
    workerInfo workerData;

    cout << "Enter n: ";
    cin >> workerData.n;
    workerData.A = new double[workerData.n];
    cout << "Elements: ";
    for (int i = 0; i < workerData.n;i++)
        cin >> workerData.A[i];

    thread t1(Work, ref(workerData));
    thread t2(Count, ref(workerData));
    mtx3.lock();

    {
        unique_lock<mutex> lock(mtx1);
        cvSort.wait(lock);
        cout << "\n Sorted Array: ";
        for (int i = 0; i < workerData.n; ++i)
            cout << workerData.A[i] << ' ';}

    mtx3.unlock();

    {
        unique_lock<mutex> lock(mtx2);
        cvKolvo.wait(lock);
        cout << "\n Count result : " << workerData.kolvo;
    }


    t2.join();
    t1.join();

    return 0;
}