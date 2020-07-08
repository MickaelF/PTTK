#include "..\include\numbergenerator.h"

NumberGenerator& NumberGenerator::get()
{
    static NumberGenerator instance; 
    return instance; 
}

NumberGenerator::NumberGenerator(): m_numberGenerator(m_randomDevice()) {}
