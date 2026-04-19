#include "your_algorithm.hpp"

#include <iostream>
#include <vector>

void print_v(std::vector<int> v) {
    for (int i : v)
        std::cout << i << " ";
    std::cout << "\n";
}

int sequencial_search(std::vector<int> &v, int a){
    for (int i = 0; i < (int)v.size(); i++) {
        if (v[i] == a)
            return i;
    }
    return -1;
}

int binary_search(std::vector<int> &v, int a){
    int inicio = 0, fim = (int)v.size() - 1;
    while (fim >= inicio) {
        int meio = (inicio + fim) / 2;
        if (v[meio] == a)
            return meio;
        else if (v[meio] < a)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }
    return -1;
}

void selection_sort(std::vector<int> &v) {
    for (int i = 0; i < (int)v.size(); i++) {
        int menor_i = i;
        for (int j = i + 1; j < (int)v.size(); j++) {
            if (v[j] < v[menor_i])
                menor_i = j;
        }
        std::swap(v[i], v[menor_i]);
    }
}

void insertion_sort(std::vector<int> &v) {
    for (int i = 1; i < (int)v.size(); i++) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && key < v[j]) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = key;
    }
}

void bubble_sort(std::vector<int> &v) {
    for (int i = 0; i < (int)v.size() - 1; i++) {
        for (int j = 0; j < (int)v.size() - i - 1; j++) {
            if (v[j] > v[j + 1])
                std::swap(v[j], v[j + 1]);
        }
    }
}

void intercalation(std::vector<int> &v, int inicio1, int inicio2, int fim2) {

    int fim1 = inicio2 - 1;
    int i = inicio1, j = inicio2, k = 0;
    std::vector<int> temp(fim2 - inicio1 + 1);

    while (i <= fim1 && j <= fim2) {
        if (v[i] <= v[j])
            temp[k++] = v[i++];
        else
            temp[k++] = v[j++];
    }
    while (i <= fim1)
        temp[k++] = v[i++];
    while (j <= fim2)
        temp[k++] = v[j++];

    for (int x = 0; x < k; x++)
        v[inicio1 + x] = temp[x];
}

void merge_sort(std::vector<int> &v) {
    merge_sort(v, 0, v.size() - 1);
}

void merge_sort(std::vector<int> &v, int esquerda, int direita) {
    if (esquerda < direita) {
        int meio = (esquerda + direita) / 2;
        merge_sort(v, esquerda, meio);
        merge_sort(v, meio + 1, direita);
        intercalation(v, esquerda, meio + 1, direita);
    }
}


int particion(std::vector<int> &v, int esquerda, int direita) {
    int pivo = v[direita];
    int i = esquerda, j = direita - 1;
    while (j >= i) {
        while (v[i] < pivo && j >= i)
            i++;
        while (v[j] > pivo && j >= i)
            j--;
        if (j >= i)
            std::swap(v[i++], v[j--]);
    }
    std::swap(v[i], v[direita]);
    return i;
}

void quick_sort(std::vector<int> &v) {
    quick_sort(v, 0, v.size() - 1);
}

void quick_sort(std::vector<int> &v, int esquerda, int direita) {
    if (esquerda < direita) {
        int i_piv = particion(v, esquerda, direita);
        quick_sort(v, esquerda, i_piv - 1);
        quick_sort(v, i_piv + 1, direita);
    }
}