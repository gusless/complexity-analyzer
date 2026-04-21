#include "complexity-analyzer.hpp"
// vvvvvvvv INSIRA SUAS BIBLIOTECAS AQUI vvvvvvvv//

#include "your_algorithm.hpp" // <---- EXEMPLO

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    Complexity_Analyzer com;

    std::cout << "\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << 
                            " ALGORITMOS DE BUSCA " <<
                 "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";

    std::cout << "**************************** BUSCA SEQUENCIAL ****************************\n\n";

    auto complexity_seq_search_random = com.analyze(
        [](std::vector<int> &v) {
            sequencial_search(v, 0);
        },
        InputType::random,
        Speed::normal,
        "sequencial_search_random.csv"
    );

    std::cout << "Complexidade da busca sequencial com valores aleatorios:\n" << complexity_seq_search_random << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    auto complexity_seq_search_best = com.analyze(
        [](std::vector<int> &v) {
            sequencial_search(v, 0);
        },
        InputType::sorted,
        Speed::normal,
        "sequencial_search_best.csv"
    );

    std::cout << "Complexidade da busca sequencial melhor caso:\n" << complexity_seq_search_best << std::endl;
    std::cout << "-------------------------------------------" << std::endl;


    auto complexity_seq_search_worst = com.analyze(
        [](std::vector<int> &v) {
            sequencial_search(v, -1);
        },
        InputType::reversed,
        Speed::slow,
        "sequencial_search_worst.csv"
    );

    std::cout << "Complexidade da busca sequencial pior caso:\n" << complexity_seq_search_worst << std::endl;
    std::cout << "-------------------------------------------" << std::endl;


    std::cout << "**************************** BUSCA BINARIA ****************************\n\n";

    auto complexity_bin_search_random = com.analyze(
        [](std::vector<int> &v) {
            int target = v[rand() % v.size()];
            binary_search(v, target);
        },
        InputType::sorted,
        Speed::normal,
        "binary_search_random.csv"
    );


    std::cout << "Complexidade da busca binaria buscando um valor aleatorio:\n" << complexity_bin_search_random << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    auto complexity_bin_search_best = com.analyze(
        [](std::vector<int> &v) {
            int mid = v[v.size() / 2];
            binary_search(v, mid);
        },
        InputType::sorted,
        Speed::normal,
        "binary_search_best.csv"
    );


    std::cout << "Complexidade da busca binaria no melhor caso:\n" << complexity_bin_search_best << std::endl;
    std::cout << "-------------------------------------------" << std::endl;


    auto complexity_bin_search_worst = com.analyze(
        [](std::vector<int> &v) {
            binary_search(v, -1);
        },
        InputType::sorted,
        Speed::normal,
        "binary_search_worst.csv"
    );


    std::cout << "Complexidade da busca binaria no pior caso:\n" << complexity_bin_search_worst << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    std::cout << "\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << 
                            " ALGORITMOS DE ORDENACAO " <<
                 "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";

    std::cout << "**************************** BUBBLE SORT ****************************\n\n";

    auto complexity_b_sort_rand = com.analyze(
        [](std::vector<int> &v) {
            bubble_sort(v);
        },
        InputType::random,
        Speed::fast,
        "bubble_sort_random.csv"
    );


    std::cout << "Complexidade do bubble sort com valores aleatorios:\n" << complexity_b_sort_rand << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    auto complexity_b_sort_best = com.analyze(
        [](std::vector<int> &v) {
            bubble_sort(v);
        },
        InputType::sorted,
        Speed::normal,
        "bubble_sort_best.csv"
    );


    std::cout << "Complexidade do bubble sort com valores ja ordenados (melhor caso):\n" << complexity_b_sort_best << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    auto complexity_b_sort_worst = com.analyze(
        [](std::vector<int> &v) {
            bubble_sort(v);
        },
        InputType::reversed,
        Speed::fast,
        "bubble_sort_worst.csv"
    );

    std::cout << "Complexidade do bubble sort com valores ordenados em ordem decrescente (pior caso):\n" << complexity_b_sort_worst << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    std::cout << "**************************** MERGE SORT ****************************\n\n";

    auto complexity_m_sort_rand = com.analyze(
        [](std::vector<int> &v) {
            merge_sort(v);
        },
        InputType::random,
        Speed::slow,
        "merge_sort_random.csv"
    );

    std::cout << "Complexidade do merge sort com valores aleatorios:\n" << complexity_m_sort_rand << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    auto complexity_m_sort_sorted = com.analyze(
        [](std::vector<int> &v) {
            merge_sort(v);
        },
        InputType::sorted,
        Speed::slow,
        "merge_sort_best.csv"
    );

    std::cout << "Complexidade do merge sort com o vetor ordenado:\n" << complexity_m_sort_sorted << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    

    auto complexity_m_sort_reversed = com.analyze(
        [](std::vector<int> &v) {
            merge_sort(v);
        },
        InputType::reversed,
        Speed::slow,
        "merge_sort_worst.csv"
    );

    std::cout << "Complexidade do merge sort com o vetor ordenado decrescentemente:\n" << complexity_m_sort_reversed << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    return 0;
}