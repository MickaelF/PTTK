#pragma once
#include <filesystem>
#include <string_view>


namespace path
{
std::filesystem::path getDataPath(std::string_view executableName, std::string_view companyName);
}