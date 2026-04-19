#pragma once

#include "complexity-enum.hpp"
#include "your_algorithm.hpp"

#include <chrono>
#include <type_traits>
#include <utility>

class Complexity_Analyzer {
private:
  	Complexity complexity;
	std::chrono::duration<double, std::nano> duration;

public:
	template<typename Func, typename... Args>
	decltype(auto) analyzer(Func&& func, Args &&...args){

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

    double get_duration() const{
		return duration.count();
	}

    Complexity get_complexity() const{
		return complexity;
	}
};
