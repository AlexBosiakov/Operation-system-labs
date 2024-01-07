#include "pch.h"
#include "CppUnitTest.h"
#include "../op_lab1/op_lab1.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test1
{
	TEST_CLASS(test1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			workerParams params;
			params.size = 5;
			params.mass = new unsigned long int[params.size] { 1, 2, 51, 60, 5 };
			params.K = 6;
			std::ostringstream oss;
			std::streambuf* pCout = std::cout.rdbuf();
			std::cout.rdbuf(oss.rdbuf());
			worker(&params);
			std::string str = oss.str();
			std::cout.rdbuf(pCout);
			Assert::AreEqual(std::string("51__60__"), str);
		}

		TEST_METHOD(TestMethod2)
		{
			workerParams params;
			params.size = 0;
			params.mass = new unsigned long int[params.size];
			params.K = 6;
			std::ostringstream oss;
			std::streambuf* pCout = std::cout.rdbuf();
			std::cout.rdbuf(oss.rdbuf());
			worker(&params);
			std::string str = oss.str();
			std::cout.rdbuf(pCout);
			Assert::AreEqual(std::string(""), str);
		}

		TEST_METHOD(TestMethod3)
		{
			workerParams params;
			params.size = 5;
			params.mass = new unsigned long int[params.size] { 1, 500, 62, 203, 14 };
			params.K = 5;
			std::ostringstream oss;
			std::streambuf* pCout = std::cout.rdbuf();
			std::cout.rdbuf(oss.rdbuf());
			worker(&params);
			std::string str = oss.str();
			std::cout.rdbuf(pCout);
			Assert::AreEqual(std::string("500__203__14__"), str);
		}
	};
}
