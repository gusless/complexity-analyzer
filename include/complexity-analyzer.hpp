#pragma once
//vvvvvvvv INSIRA SUAS BIBLIOTECAS AQUI vvvvvvvv//

#include "your_algorithm.hpp" // <---- EXEMPLO

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
#include <chrono>
#include <cmath>
#include <type_traits>
#include <utility>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>

// enum contendo complexidades
enum class Complexity {
    constant, // O(1)
    n,        // O(n)
    n_2,      // O(n^2)
    n_3,      // O(n^3)
    nlogn,    // O(n*log(n))
    logn,     // O(log(n))
    _2_n,     // O(2^n)
    _3_n,     // O(3^n)
    fat_n,    // O(n!)
};

// enum de velocidade do algoritmo de analise de complexidade, ela define tambem a sua precisao
// ela quem define qual vetor de tamanhos e os tempos do algoritmo
// quanto mais rapido mais ele pode errar a complexidade
enum class Speed {
    fast,
    normal,
    slow
};

// enum que define como serao gerados os vetores, podendo analisar o algoritmo
// de diversas formas diferentes, alem disso alguns algoritmos dependem dos dados ja ordenados
enum class InputType {
    sorted,
    reversed,
    random
};

// struct auxiliar para configuracao do vetor
struct Config {
    std::vector<int> sizes;
    double min_time;
};

/*
Classe principal do projeto, ela analisa o algoritmo definindo a sua complexidade assintotica,
baseando-se em tamanhos diferentes e tempos de execucao do algoritmo escolhido.
Vale ressaltar que o algoritmo tem a sua precisao, e para isso houve uma tentativa de melhorar, isso pode deixar a analise lenta,
porem mais precisa, e ainda assim o algoritmo pode errar, para o merge sort por exemplo, algumas vezes resultava em um O(n), 
tambem nao deve-se sempre levar a serio uma analise na qual foi escolhida o InputType::random, 
pois ele pode gerar um possivel melhor caso para o algoritmo e terminar em O(1), entre outros casos.
*/
class Complexity_Analyzer {
  private:
    Complexity complexity; // complexidade do algoritmo
    Config config; // configuracao do vetor
    std::vector<double> times; // tempos gerados

    // metodo para definiar a configuracao do vetor, o enum de velocidade ira definir o tamanho e o tempo 
    Config get_config(Speed speed) {
        switch (speed) {
            case Speed::fast:
                return {
                    {2000, 4000, 8000, 16000},
                    0.02
                };
            case Speed::normal:
                return {
                    {10000, 20000, 40000, 80000},
                    0.05
                };
            case Speed::slow:
                return {
                    {20000, 40000, 80000, 160000, 320000},
                    0.05
                };
        }

        return {};
    }

    // metodo pra gerar o vetor da forma escolhida, ordenado crescente ou decrescente ou valores aleatorios
    std::vector<int> generate_input(int n, InputType type) {
        std::vector<int> v(n);

        switch (type) {
            case InputType::sorted:
                for (int i = 0; i < n; i++)
                    v[i] = i;
                break;
            case InputType::reversed:
                for (int i = 0; i < n; i++)
                    v[i] = n - i - 1;
                break;
            case InputType::random:
                for (int i = 0; i < n; i++)
                    v[i] = rand();
                break;
        }
        return v;
    }

    // faz uma estimativa da complexidade assintotica com os tamanhos e os tempos medidos
    Complexity estimate_complexity(const std::vector<int> &sizes, const std::vector<double> &times) {    
        {
            double slope = 0;

            // calcula a inclinacao media (dt/dn), o quanto o tempo cresce por valor de n
            for (size_t i = 1; i < sizes.size(); i++) {
                double dn = sizes[i] - sizes[i - 1]; //variacoa de n
                double dt = times[i] - times[i - 1]; //variacao de t

                slope += dt / dn; // taxa de crescimento local
            }

            //media das inclinacoes
            slope /= (sizes.size() - 1);

            // se nao cresce entao eh O(1)
            if (slope < 1e-9)
                return Complexity::constant;
        }  
        
        // funcao pra medir o coeficiente de variacao
        auto compute_cv = [](const std::vector<double>& v) {
            double mean = 0;

            //media
            for (double x : v) mean += x;
            mean /= v.size();

            double var = 0;

            //variancia
            for (double x : v) var += (x - mean) * (x - mean);
            var /= v.size();

            double stddev = std::sqrt(var);
            
            //coef de variacao = desvio padrao / media
            return stddev / mean;
        };

        // para cada f(n) eh feito -> times[i] / f(n)
        // se O(f(n)) -> times ~ c * f(n)
        // -> time / f(n) ~ constante
        auto test_model = [&](auto f) {
            std::vector<double> ratios;
            for (size_t i = 0; i < sizes.size(); i++) {
                double fn = f(sizes[i]);
                // != log(0)
                if (fn <= 0) continue;
                ratios.push_back(times[i] / fn);
            }
            return compute_cv(ratios);
        };

        // testar as diferentes complexidades, cada variavel eh um erro
        double cv_n     = test_model([](double n) { return n; });
        double cv_nlogn = test_model([](double n) { return n * std::log2(n); });
        double cv_n2    = test_model([](double n) { return n * n; });
        double cv_n3    = test_model([](double n) { return n * n * n; });
        double cv_logn  = test_model([](double n) { return std::log2(n); });

        // escolher do melhor modelo, definindp a complexidade
        std::vector<std::pair<Complexity, double>> results = {
            {Complexity::logn,  cv_logn},
            {Complexity::n,     cv_n},
            {Complexity::nlogn, cv_nlogn},
            {Complexity::n_2,   cv_n2},
            {Complexity::n_3,   cv_n3}
        };

        //retornando a complexidade com o menor erro
        return std::min_element(results.begin(), results.end(),
            [](auto &a, auto &b) { return a.second < b.second; })->first;
    }

  public:

    //serve para converter o enun de complexidade para uma string
	inline const char* to_string(Complexity c) {
		switch (c) {
            case Complexity::constant: return "O(1) or O(log n)"; // <- nesse caso por imprecisao do algoritmo, algumas vezes o O(log n) estava caindo em constant
			case Complexity::n: return "O(n)";                    // por isso, achei melhor colocar dessa forma
			case Complexity::n_2: return "O(n^2)";
			case Complexity::n_3: return "O(n^3)";
			case Complexity::nlogn: return "O(n log n)";
			case Complexity::logn: return "O(log n)";
			case Complexity::_2_n: return "O(2^n)"; // <- daqui pra baixo o meu algoritmo nao detecta, coloquei so de representatividade
			case Complexity::_3_n: return "O(3^n)";
			case Complexity::fat_n: return "O(n!)";
			default: return "unknown";
    	}
	}	

    // principal metodo do algoritmo, ele gera entradas de diferentes tamanhos, executa a funcao para cada tamanho,
    // mede os tempos medios, tenta definir uma complexidade, nao sendo a complexidade real, mas uma estimada
    // e, caso o usuario queira, gera um arquivo com os dados
    template<typename Func>
    std::string analyze(
        Func&& func, // funcao a ser analisada
        InputType input_type = InputType::random, // o usuario decide a ordenacao do vetor
        Speed speed = Speed::normal, // o usuario decide a velocidade/precisao do algoritmo
        const std::string& filename = "-1", // caso o usuario queira salvar um csv, basta colocar uma string.csv, qualquer uma tirando "-1"
        const std::string& folder = "results"
    ) {
        
        //configuracao definida pelo usuario baseada na velocidade escolhida
        this->config = get_config(speed);

        //limpa as medicoes anteriores
        times.clear();

        // para cada tamanho, o algoritmo ira medir os tempos
        for (int n : config.sizes) {
            { // serve pra evitar overhead
                auto input = generate_input(n, input_type);
                func(input);
            }
            double total_time = 0.0;
            int count = 0;

            // enquanto nao bater o tempo minimo, acumula o tempo de funcionamento do algoritmo
            while (total_time < config.min_time) {

                //gerar uma nova entrada por execucao
                auto input = generate_input(n, input_type);

                //tempo no momento que inicia o algoritmo a ser analisado
                auto start = std::chrono::high_resolution_clock::now();

                //execucao do algoritmo
                func(input);

                //tempo no momento que o algoritmo termina de ser executado
                auto end = std::chrono::high_resolution_clock::now();

                //tempo do algoritmo = tempo final - tempo inicial
                std::chrono::duration<double> dur = end - start;

                //acumulacao do tempo total
                total_time += dur.count();
                count++;
            }

            //media das execucoes
            double avg_time = total_time / count;
            times.push_back(avg_time); //armazena o tempo medio por tamanho do vetor
        }

        // com os tempos coletados eh inferido a ordem de crescimento
        this->complexity = estimate_complexity(config.sizes, times);

        if (filename != "-1") //gera um csv se o usuario digitar algo (que nao seja "-1")
            save_to_csv(filename, folder);

        return to_string(complexity); // retorna a complexidade em string
    }

    //metodo para salvar em csv pra gerar os graficos (usei python por ser mais convencional, e por isso nao ha nenhum gerador de grafico aqui)
    void save_to_csv(const std::string& filename, const std::string& folder = "results") {

        if (!std::filesystem::exists(folder))
            std::filesystem::create_directory(folder);
        std::string full_path = folder + "/" + filename;

        std::ofstream file(full_path);

        file << "n,time,complexity\n";

        for (size_t i = 0; i < config.sizes.size(); i++) {
            file << config.sizes[i] << "," << times[i] << "," 
                << to_string(complexity) << "\n";
        }

        file.close();
    }

    //get para retornar a complexidade
    Complexity get_complexity() const {
        return complexity;
    }
};













//q trabalho dificil cara >:c