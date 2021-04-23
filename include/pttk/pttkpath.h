#pragma once
#include <filesystem>
#include <string_view>

namespace pttkPath
{
    std::wstring getDataPathStr(std::wstring_view executableName,
                                std::wstring_view companyName);
    std::filesystem::path getDataPath(std::wstring_view executableName,
                                      std::wstring_view companyName);
    std::wstring getDocumentFolderPathStr(std::wstring_view executableName,
                                          std::wstring_view companyName);
    std::filesystem::path getDocumentFolderPath(std::wstring_view executableName,
                                                std::wstring_view companyName);
} // namespace pttkPath