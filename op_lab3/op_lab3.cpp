#include <iostream>
#include <windows.h>
#include <string>
#include <cmath>

using namespace std;

struct workerInfo {
    _int16* A;
    int n;
    int kolvo;
    HANDLE eventHandleSort;
    HANDLE eventHandlekolvo;
    CRITICAL_SECTION cs;
    CRITICAL_SECTION cs2;

};


void Work(LPVOID params) {
    workerInfo& info = *static_cast<workerInfo*>(params);
    int interval;
    cout << "Enter interval(ms): ";
    cin >> interval;
    int k = 0;
    for (int i = 0; i < info.n; ++i) {

        if (info.A[i] > 0)
        {
            _int16 e = info.A[i];
            info.A[i] = sqrt(e);
            swap(info.A[k], info.A[i]);
            k++;
        }

        Sleep(interval);
    }
    SetEvent(info.eventHandleSort);
}

void Count(LPVOID params) {
    workerInfo& info = *static_cast<workerInfo*>(params);
    EnterCriticalSection(&info.cs2);
    info.kolvo = 0;
    for (int i = 0; i < info.n; ++i) {
        if (info.A[i] < 0)
            info.kolvo += 1;
    }
    LeaveCriticalSection(&info.cs2);
    SetEvent(info.eventHandlekolvo);
}

int main()
{

    workerInfo workerData;
    InitializeCriticalSection(&workerData.cs);
    InitializeCriticalSection(&workerData.cs2);
    workerData.eventHandleSort = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (workerData.eventHandleSort == NULL) {
        return GetLastError();
    }

    cout << "Enter n: ";
    cin >> workerData.n;
    workerData.A = new _int16[workerData.n];
    cout << "Elements: ";
    for (int i = 0; i < workerData.n;i++)
        cin >> workerData.A[i];

    workerData.eventHandlekolvo = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (workerData.eventHandlekolvo == NULL) {
        return GetLastError();
    }
    HANDLE hThread[2];
    DWORD dwThread[2];
    hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Work,
        (LPVOID)(&workerData), 0, &dwThread[0]);
    if (hThread[0] == NULL) {
        return GetLastError();
    }
    hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Count,
        (LPVOID)(&workerData), 0, &dwThread[1]);
    if (hThread[1] == NULL) {
        return GetLastError();
    }
    EnterCriticalSection(&workerData.cs2);
    WaitForSingleObject(workerData.eventHandleSort, INFINITE);

    cout << "\n Sorted Array: ";
    for (int i = 0; i < workerData.n; ++i) {
        cout << workerData.A[i] << ' ';
    }
    LeaveCriticalSection(&workerData.cs2);
    WaitForSingleObject(workerData.eventHandlekolvo, INFINITE);
    cout << "\n Count result : " << workerData.kolvo;


    DeleteCriticalSection(&workerData.cs);
    DeleteCriticalSection(&workerData.cs2);
    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);
    CloseHandle(workerData.eventHandleSort);
    CloseHandle(workerData.eventHandlekolvo);
    return 0;
}

