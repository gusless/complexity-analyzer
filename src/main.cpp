#include "complexity-analyzer.hpp"
//vvvvvvvv INSIRA SUAS BIBLIOTECAS AQUI vvvvvvvv//

#include "your_algorithm.hpp" // <---- EXEMPLO

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
#include <iostream>
#include <vector>
#include <iomanip>

int main() {
    Complexity_Analyzer com;

    auto complexity = com.analyze([&](int n) {
        std::vector<int> v(n);

        for (auto& x : v) x = rand(); // vetores desorganizados
        
        //vvvvvvvv INSIRA SUA FUNÇÃO AQUI vvvvvvvv//

        merge_sort(v); // <---- EXEMPLO

        //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
    });

    std::cout << complexity << std::endl;

    return 0;
}