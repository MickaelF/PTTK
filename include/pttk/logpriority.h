#pragma once
#include <string_view>

namespace LogPriority
{
enum class Priorities
{
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    Fatal = 4,
    Remember = 5,
    Execution = 6
};

inline constexpr int enumMaxNumber()
{
    return static_cast<int>(Priorities::Execution) + 1;
}

inline constexpr std::string_view enumToStr(Priorities p)
{
    switch (p)
    {
        case Priorities::Debug: return "Debug";
        case Priorities::Info: return "Info";
        case Priorities::Warning: return "Warning";
        case Priorities::Error: return "Error";
        case Priorities::Fatal: return "Fatal";
        case Priorities::Remember: return "Remember";
        case Priorities::Execution: return "Execution";
        default: return "Unknown";
    }
}
} // namespace LogPriority