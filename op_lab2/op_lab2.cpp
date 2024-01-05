#include <iostream>
#include <windows.h>

void startParent() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USEFILLATTRIBUTE;
    si.dwFillAttribute = FOREGROUND_RED;
    if (CreateProcess(L"Debug\\Parent.exe", NULL, NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cout << "ok";
    }
    else {
        std::cout << "not ok";
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

int main()
{
    startParent();
	return 0;
}

