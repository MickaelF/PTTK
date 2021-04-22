#pragma once
#include <filesystem>
#include <string_view>

namespace pttkPath
{
std::filesystem::path getDataPath(std::string_view executableName,
                                  std::string_view companyName);
std::filesystem::path getDocumentFolderPath(std::string_view executableName,
                                            std::string_view companyName);
} // namespace pttkPath