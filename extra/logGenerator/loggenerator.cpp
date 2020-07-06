#include "loggenerator.h"

#include <random>
#include <vector>

#include "log.h"
#include "logger.h"

LogGenerator::LogGenerator(std::filesystem::path outputPath, int nbLines) : m_nbLines(nbLines)
{
    m_outStream.open(outputPath, std::ios_base::out | std::ios_base::app);
    if (!m_outStream.is_open()) throw std::runtime_error("Cannot create output file.");

    Logger::swapStream(m_outStream);
}

void LogGenerator::exec()
{
    const std::vector<std::string> randomFileName {};
    std::random_device rd; // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    for (int i = 0; i < m_nbLines; ++i)
    {
        std::uniform_int_distribution distrib(static_cast<int>(Log::Priority::Debug),
                                              static_cast<int>(Log::Priority::Execution));
        Log l(static_cast<Log::Priority>(distrib(gen)), "Test.cpp", 32);
    }
    Logger::swapStream(m_outStream);
}