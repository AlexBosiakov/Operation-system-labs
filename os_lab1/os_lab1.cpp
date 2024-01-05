#include <Windows.h>
#include <iostream>

struct workerParams {
	unsigned long int* mass;
	int size;
	int K;
};

DWORD WINAPI worker(LPVOID ptParams) {
	workerParams* params = (workerParams*)ptParams;

	for (int i = 0; i < params->size; ++i) {
		int k = 0, a = params->mass[i];;
		while (a > 0)
		{
			k += a % 10;
			a /= 10;
		}
		if (params->K == k) {
			std::cout << params->mass[i] << "__";
		}
		Sleep(20);
	}
	return 0;
}


int main()
{

	workerParams params;
	std::cout << "Enter number of elements: ";
	std::cin >> params.size;
	params.mass = new unsigned long int[params.size];
	std::cout << "Enter elements: ";
	for (int i = 0; i < params.size; ++i) {
		std::cin >> params.mass[i];
	}
	std::cout << "K: ";
	std::cin >> params.K;
	HANDLE hThread;
	DWORD IDThread;
	hThread = CreateThread(NULL, 0, worker, (void*)(&params), 0, &IDThread);
	if (hThread == NULL) {
		return GetLastError();
	}
	SuspendThread(hThread);
	Sleep(20);
	ResumeThread(hThread);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}