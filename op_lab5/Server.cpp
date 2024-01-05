#include <iostream>
#include <windows.h>
#include <string>

int main()
{
    std::string *str = new std::string, processedStr;
    std::cout << sizeof(std::string);
    std::cout << "Enter string: ";
    std::cin >> *str;
    HANDLE hEnableConsumeRead = CreateEvent(NULL, FALSE, FALSE, "EnableConsumeRead");
    HANDLE hEnableServerRead = CreateEvent(NULL, FALSE, FALSE, "EnableServerRead");
    if (hEnableConsumeRead == NULL || hEnableServerRead == NULL) {
        std::cout << "Events weren't created";
        return GetLastError();
    }

    HANDLE hWritePipe1, hWritePipe2, hReadPipe1, hReadPipe2;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0)) {
        std::cout << "Pipe 1 wasn't created";
        return GetLastError();
    }
    if (!CreatePipe(&hReadPipe2, &hWritePipe2, &sa, 0)) {
        std::cout << "Pipe 2 wasn't created";
        return GetLastError();
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char lpszComLine[80];
    wsprintf(lpszComLine, "%d %d", (int)hWritePipe2, (int)hReadPipe1);
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    if (!CreateProcess("Debug\\Consume.exe", lpszComLine, NULL, NULL, TRUE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cout << "\nConsume process is not created\n";
        return GetLastError();
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    DWORD dwBytesWrite;
    if (!WriteFile(hWritePipe1, str, sizeof(std::string), &dwBytesWrite, NULL)) {
        std::cout << "\nWrite failed\n";
        return GetLastError();
    }
    SetEvent(hEnableConsumeRead);

    WaitForSingleObject(hEnableServerRead, INFINITE);
    DWORD dwBytesRead;
    if (!ReadFile(hReadPipe2, &processedStr, sizeof(std::string), &dwBytesRead, NULL)) {
        std::cout << "\nRead failed\n";
        return GetLastError();
    }
    std::cout << "\n\n\n" << processedStr;
    std::string s;
    getline(std::cin, s);
    CloseHandle(hEnableConsumeRead);
    CloseHandle(hEnableServerRead);
    CloseHandle(hWritePipe1);
    CloseHandle(hWritePipe2);
    CloseHandle(hReadPipe1);
    CloseHandle(hReadPipe2);
    return 0;
}
