#include "complexity-enum.hpp"
#include "complexity-analyzer.hpp"
#include "your_algorithm.hpp"

#include <iostream>
#include <vector>
#include <iomanip>

int main() {
    Complexity_Analyzer com;

    std::vector<int> v;
    for (int i = 1000; i >= 0; i--) {
        v.push_back(i);
    }

    com.analyzer(bubble_sort, v);
    double time = com.get_duration();

    std::cout << "tempo de execucao: " << std::fixed  << std::setprecision(2) << time << std::endl;

    return 0;
}