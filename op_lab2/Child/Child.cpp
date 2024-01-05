#include <iostream>
#include <windows.h>
#include <string>

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i) {
        int num = atoi(argv[i]);
        if (num % 9 == 0 && num < 0)
            printf("%d ", num);
        Sleep(30);
    }
    std::string temp;
    std::getline(std::cin, temp);
    return 0;
}
