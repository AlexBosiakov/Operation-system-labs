#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <filesystem>
#include <mutex>
#include <cmath>

std::mutex mtx;

void process_file(const std::string& filename, double& total) {
    std::ifstream file(filename);
    if (file.is_open()) {
        int operation;
        file >> operation;

        std::vector<double> numbers;
        double num;
        while (file >> num) {
            numbers.push_back(num);
        }

        double res = 0;

        if (operation == '1')	//summation
        {
            double res = 0;
            for (int i = 0; i < numbers.size(); ++i)
                res += numbers[i];            
        }

        if (operation == '2')	//multiplication
        {
            double res = 1;
            for (int i = 0; i < numbers.size(); ++i)           
                res *= numbers[i];
        }

        if (operation == '3')	//square summation
        {
            double res = 0;
            for (int i = 0; i < numbers.size(); ++i)
                res += numbers[i] * numbers[i];
        }

        mtx.lock();
        total += res;
        mtx.unlock();

        file.close();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <directory> <num_threads>" << std::endl;
        return 1;
    }

    std::string directory = argv[1];
    int num_threads = std::stoi(argv[2]);

    std::vector<std::thread> threads;
    double total = 0;

    for (auto& p : std::filesystem::directory_iterator(directory)) {
        if (p.path().extension() == ".dat") {
            threads.emplace_back(process_file, p.path().string(), std::ref(total));
            if (threads.size() >= num_threads) {
                for (auto& t : threads) {
                    t.join();
                }
                threads.clear();
            }
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    std::ofstream outfile("out.dat");
    if (outfile.is_open()) {
        outfile << total << std::endl;
        outfile.close();
    }

    return 0;
}
