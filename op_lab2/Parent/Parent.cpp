#include <iostream>
#include <windows.h>
#include <string>

void startChild(int* arr, int size) {
    std::string args = "";
    for (int i = 0; i < size; ++i) {
        args += " " + std::to_string(arr[i]);
    }
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USEFILLATTRIBUTE;
    si.dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    if (CreateProcess("Debug\\Child.exe", (char*)args.c_str(), NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cout << "second ok";
    }
    else std::cout << "second not ok";

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

int main(int argc, char* argv[])
{
    int n;
    std::cout << "Enter size of array: ";
    std::cin >> n;
    int* arr = new int[n];
    std::cout << "Enter elements of array: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    startChild(arr, n);
    std::string temp;
    std::getline(std::cin, temp);
    return 0;
}