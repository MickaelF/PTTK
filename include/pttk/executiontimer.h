#pragma once
#include <chrono>
#include <string>
#include "log.h"

class ExecutionTimer
{
public:
    ExecutionTimer() = delete;
    ExecutionTimer(const ExecutionTimer&) = delete;
    ExecutionTimer(ExecutionTimer&&) = delete;
    ExecutionTimer& operator=(const ExecutionTimer&) = delete;

    ExecutionTimer(std::string&& name) : m_name(name), m_start(std::chrono::system_clock::now()) {}

    ~ExecutionTimer()
    {
        auto end = std::chrono::system_clock::now();
        std::string unity = "ms\n"; 
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
        if (duration <= 1)
        {
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_start).count();
            unity = "ns\n";
        }
        std::stringstream ss;
        ss << m_name << ": " << duration << unity;
        lExecution << ss.str();
    }

private:
    const std::string m_name;
    std::chrono::time_point<std::chrono::system_clock> m_start;
};
