#pragma once

#include <vector>


int sequencial_search(std::vector<int> &v, int a);
int binary_search(std::vector<int> &v, int a);
void print_v(std::vector<int> v);
void selection_sort(std::vector<int> &v);
void insertion_sort(std::vector<int> &v);
void bubble_sort(std::vector<int> &v);
void intercalation(std::vector<int> &v, int inicio1, int inicio2, int fim2);
void merge_sort(std::vector<int> &v, int esquerda, int direita);
void merge_sort(std::vector<int> &v);
int particion(std::vector<int> &v, int esquerda, int direita);
void quick_sort(std::vector<int> &v, int esquerda, int direita);
void quick_sort(std::vector<int> &v);

