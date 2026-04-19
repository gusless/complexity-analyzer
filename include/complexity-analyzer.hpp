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

enum class Complexity {
    n,     // O(n)
    n_2,   // O(n^2)
    n_3,   // O(n^3)
    nlogn, // O(n*log(n))
    logn,  // O(log(n))
    _2_n,  // O(2^n)
    _3_n,  // O(3^n)
    fat_n, // O(n!)
};

class Complexity_Analyzer {
  private:
    Complexity complexity;
    std::chrono::duration<double, std::nano> duration;
    std::vector<int> sizes = {1000, 2000, 4000, 8000, 16000};
    std::vector<double> times;

    Complexity estimate_complexity(const std::vector<int> &sizes, const std::vector<double> &times) {

        std::vector<double> ks;

        for (size_t i = 1; i < sizes.size(); i++) {
            double t1 = times[i - 1];
            double t2 = times[i];

            double n1 = sizes[i - 1];
            double n2 = sizes[i];

            if (t1 <= 0 || t2 <= 0)
                continue;

            double k = log(t2 / t1) / log(n2 / n1);
			ks.push_back(k);
        
        }

        double avg_k = 0;
        for (double k : ks)
            avg_k += k;
        avg_k /= ks.size();

        if (avg_k < 0.3)
            return Complexity::logn;
        else if (avg_k < 1.2)
            return Complexity::n;
        else if (avg_k < 1.6)
            return Complexity::nlogn;
        else if (avg_k < 2.4)
            return Complexity::n_2;
        else if (avg_k < 3.2)
            return Complexity::n_3;
        else
            return Complexity::_2_n;
    }

  public:

	inline const char* to_string(Complexity c) {
		switch (c) {
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

    template <typename Func>
	
    std::string analyze(Func&& func) {

		times.clear();

		for (int n : sizes) {

			int repeat = std::max(10, 100000 / n);
			std::vector<double> samples;

			// warm-up leve
			func(n);

			for (int i = 0; i < repeat; i++) {

				auto start = std::chrono::steady_clock::now();

				func(n);

				auto end = std::chrono::steady_clock::now();

				std::chrono::duration<double> dur = end - start;

				samples.push_back(dur.count());
			}

			std::sort(samples.begin(), samples.end());

			int trim = samples.size() * 0.2;

			double sum = 0;
			int count = 0;

			for (size_t i = trim; i < samples.size() - trim; i++) {
				sum += samples[i];
				count++;
			}

			double time = sum / count;

			times.push_back(time); // 🔥 ESSENCIAL
		}

		this->complexity = estimate_complexity(sizes, times);

		return to_string(complexity);
	}

    template <typename Func, typename... Args>
    decltype(auto) function_time(Func &&func, Args &&...args) {

        using ReturnType = std::invoke_result_t<Func, Args...>;

        auto start = std::chrono::high_resolution_clock::now();

        // if constexpr (std::is_void_v<decltype(func(args...))>) {
        if constexpr (std::is_void_v<ReturnType>) {

            // func(args...);
            std::forward<Func>(func)(std::forward<Args>(args)...);
            auto end = std::chrono::high_resolution_clock::now();

            this->duration = end - start;
        } else {

            // auto res = func(args...);
            auto result = std::forward<Func>(func)(std::forward<Args>(args)...);
            auto end = std::chrono::high_resolution_clock::now();

            this->duration = end - start;

            return result;
        }
    }

    double get_duration() const {
        return duration.count();
    }

    Complexity get_complexity() const {
        return complexity;
    }
};
