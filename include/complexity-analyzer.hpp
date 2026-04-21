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

enum class Speed {
    fast,
    normal,
    slow
};

enum class InputType {
    sorted,
    reversed,
    random
};

struct Config {
    std::vector<int> sizes;
    double min_time;
};

class Complexity_Analyzer {
  private:
    Complexity complexity;
    Config config;
    std::vector<double> times;

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

    Complexity estimate_complexity(const std::vector<int> &sizes, const std::vector<double> &times) {    
        {
            double slope = 0;

            for (size_t i = 1; i < sizes.size(); i++) {
                double dn = sizes[i] - sizes[i - 1];
                double dt = times[i] - times[i - 1];

                slope += dt / dn;
            }

            slope /= (sizes.size() - 1);

            if (slope < 1e-9)
                return Complexity::constant;
        }  
                                       
        auto compute_cv = [](const std::vector<double>& v) {
            double mean = 0;
            for (double x : v) mean += x;
            mean /= v.size();

            double var = 0;
            for (double x : v) var += (x - mean) * (x - mean);
            var /= v.size();

            double stddev = std::sqrt(var);
            return stddev / mean;
        };

        auto test_model = [&](auto f) {
            std::vector<double> ratios;
            for (size_t i = 0; i < sizes.size(); i++) {
                double fn = f(sizes[i]);
                if (fn <= 0) continue;
                ratios.push_back(times[i] / fn);
            }
            return compute_cv(ratios);
        };

        double cv_n     = test_model([](double n) { return n; });
        double cv_nlogn = test_model([](double n) { return n * std::log2(n); });
        double cv_n2    = test_model([](double n) { return n * n; });
        double cv_n3    = test_model([](double n) { return n * n * n; });
        double cv_logn  = test_model([](double n) { return std::log2(n); });

        std::vector<std::pair<Complexity, double>> results = {
            {Complexity::logn,  cv_logn},
            {Complexity::n,     cv_n},
            {Complexity::nlogn, cv_nlogn},
            {Complexity::n_2,   cv_n2},
            {Complexity::n_3,   cv_n3}
        };

        return std::min_element(results.begin(), results.end(),
            [](auto &a, auto &b) { return a.second < b.second; })->first;
    }


  public:

	inline const char* to_string(Complexity c) {
		switch (c) {
            case Complexity::constant: return "O(1) or O(log n)";
			case Complexity::n: return "O(n)";
			case Complexity::n_2: return "O(n^2)";
			case Complexity::n_3: return "O(n^3)";
			case Complexity::nlogn: return "O(n log n)";
			case Complexity::logn: return "O(log n)";
			case Complexity::_2_n: return "O(2^n)";
			case Complexity::_3_n: return "O(3^n)";
			case Complexity::fat_n: return "O(n!)";
			default: return "unknown";
    	}
	}	

    template<typename Func>

    std::string analyze(Func&& func, 
        InputType input_type = InputType::random, 
        Speed speed = Speed::normal,
        //bool generate = false,
        const std::string& filename = "-1", 
        const std::string& folder = "results"
    ) {

        this->config = get_config(speed);
        times.clear();

        for (int n : config.sizes) {
            {
                auto input = generate_input(n, input_type);
                func(input);
            }
            double total_time = 0.0;
            int count = 0;

            while (total_time < config.min_time) {

                auto input = generate_input(n, input_type);

                auto start = std::chrono::high_resolution_clock::now();

                func(input);

                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> dur = end - start;

                total_time += dur.count();
                count++;
            }

            double avg_time = total_time / count;
            times.push_back(avg_time);
        }

        this->complexity = estimate_complexity(config.sizes, times);

        if (filename != "-1")
            save_to_csv(filename, folder);

        return to_string(complexity);
    }

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


    Complexity get_complexity() const {
        return complexity;
    }
};
