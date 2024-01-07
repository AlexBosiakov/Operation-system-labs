#include "pch.h"
#include "CppUnitTest.h"
#include "..\\op_lab6\\op_lab6.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test6
{
	TEST_CLASS(Test6)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			workerInfo params;
			params.n = 5;
			params.A = new int[params.n] { 4, -4, 64, 51, 5 };

			TestProcess(params.n, params.A, 0, "2 8 7 2 -4 ", 1);
		}

		TEST_METHOD(TestMethod2)
		{
			workerInfo params;
			params.n = 1;
			params.A = new int[params.n] { 1 };

			TestProcess(params.n, params.A, 0, "1 ", 1);
		}

		TEST_METHOD(TestMethod3)
		{
			workerInfo params;
			params.n = 3;
			params.A = new int[params.n] { 1, 2, -1 };

			TestProcess(params.n, params.A, 0, "1 1 -1 ", 3);
		}

		TEST_METHOD(TestMethod4)
		{
			workerInfo params;
			params.n = 4;
			params.A = new int[params.n] { -3, -4, -5, -6 };

			TestProcess(params.n, params.A, 0, "-3 -4 -5 -6 ", 2);
		}

		TEST_METHOD(TestMethod5)
		{
			workerInfo params;
			params.n = 4;
			params.A = new int[params.n] {  4, -8, 5, -1 };

			TestProcess(params.n, params.A, 0, "2 2 -8 -1 ", 1);
		}

		void TestProcess(int n, int* a, int time, std::string expected_array, int expected_result) {
			HANDLE hReadPipe, hWritePipe;
			HANDLE hReadTestPipe, hWriteTestPipe;
			SECURITY_ATTRIBUTES saAttr;

			saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			saAttr.bInheritHandle = TRUE;
			saAttr.lpSecurityDescriptor = NULL;

			if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
				Assert::Fail(L"Error while creating pipe");
			}

			if (!CreatePipe(&hReadTestPipe, &hWriteTestPipe, &saAttr, 0)) {
				Assert::Fail(L"Error while creating pipe");
			}

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.hStdError = hWriteTestPipe;
			si.hStdOutput = hWriteTestPipe;
			si.hStdInput = hReadPipe;
			si.dwFlags |= STARTF_USESTDHANDLES;

			if (!CreateProcessA(reinterpret_cast<LPCSTR>("C:\\Colloquium3\\op_lab6\\x64\\Debug\\op_lab6.exe"), NULL, NULL, NULL,
				TRUE, 0, NULL, NULL, reinterpret_cast<LPSTARTUPINFOA>(&si), &pi)) {
				Assert::Fail(L"Failed to create process");
			}

			CloseHandle(hReadPipe);
			CloseHandle(hWriteTestPipe);

			char* input = new char[10 * (n + 3) + n + 3 + 1];
			char* element = new char[11];

			wsprintfA(input, "%d ", n);
			for (int i = 0; i < n; ++i)
			{
				wsprintfA(element, "%d ", a[i]);
				strcat(input, element);
			}
			wsprintfA(element, "%d ", time);
			strcat(input, element);

			DWORD dwBytesWritten;

			if (!WriteFile(hWritePipe, input, strlen(input),
				reinterpret_cast<LPDWORD>(&dwBytesWritten), NULL))
			{
				Assert::Fail(L"Error while writing pipe");
			}

			delete[] input;
			delete[] element;

			WaitForSingleObject(pi.hProcess, INFINITE);

			char buf[4096];
			DWORD dwBytesRead;

			ReadFile(hReadTestPipe, buf, sizeof(buf), &dwBytesRead, NULL);
			int i = 54;
			for (; buf[i] != '\r'; ++i)
			{
				Assert::AreEqual(expected_array[i - 54], buf[i]);
			}

			i += 18;
			int j = i;
			char result[10];
			for (; buf[i] != -52; ++i)
			{
				result[i - j] = buf[i];
			}

			Assert::AreEqual(expected_result, atoi(result));

			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	};
}