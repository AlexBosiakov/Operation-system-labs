#include <iostream>
#include <Windows.h>
#include <string>

int main(int argc, char *argv[])
{
    std::string str, processedStr;
    HANDLE hWritePipe, hReadPipe;
    HANDLE hEnableServerRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, "EnableServerRead");
    HANDLE hEnableConsumeRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, "EnableConsumeRead");
    if (hEnableConsumeRead == NULL || hEnableServerRead == NULL) {
        std::cout << "Events weren't created";
        return GetLastError();
    }
    hWritePipe = (HANDLE)atoi(argv[0]);
    hReadPipe = (HANDLE)atoi(argv[1]);
    DWORD dwBytesWrite;

    WaitForSingleObject(hEnableConsumeRead, INFINITE);
    DWORD dwBytesRead;
    if (!ReadFile(hReadPipe, &str, sizeof(str), &dwBytesRead, NULL)) {
        std::cout << "\nRead failed\n";
        return GetLastError();
    }
    std::cout << str;
    processedStr = str;
    if (!WriteFile(hWritePipe, &processedStr, sizeof(processedStr), &dwBytesWrite, NULL)) {
        std::cout << "\nWrite failed\n";
        return GetLastError();
    }
    SetEvent(hEnableServerRead);
    std::string s;
    getline(std::cin, s);
    
    CloseHandle(hEnableConsumeRead);
    CloseHandle(hEnableServerRead);
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    return 0;
}
