#pragma once
#include <type_traits>
#include <random>
#include <pttk/macroutils.h>

class NumberGenerator
{
public: 
    
    /*template<typename T> 
    static T generateNumberBetween(T min, T max) 
    {
        static_assert(std::is_fundamental_v<T>, "Type must be a fundamental type");

    }*/

    static int generateBetween(int min, int max)
    {
        return std::uniform_int_distribution(min, max)(get().m_numberGenerator);
    }

private: 
    static NumberGenerator& get();
	explicit NumberGenerator(); 
    DELETE_COPY_CONSTR_ASSIGN(NumberGenerator)
    
	std::random_device m_randomDevice; // Will be used to obtain a seed for the random number engine
    std::mt19937 m_numberGenerator; // Standard mersenne_twister_engine seeded with rd()
};