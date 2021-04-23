#include "pttk/pttkpath.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <ShlObj.h>
#include <sstream>
#include <locale>
#include <codecvt>
#endif

namespace
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    std::wstring getWindowPath(const GUID folderId)
    {
        wchar_t *path{nullptr};
        SHGetKnownFolderPath(folderId, KF_FLAG_DEFAULT,
                             NULL, &path);
        std::wstringstream ss;
        ss << path;
        CoTaskMemFree(static_cast<void *>(path));
        return ss.str();
    }
#endif
}

namespace pttkPath
{
    std::wstring getDataPathStr(std::string_view executableName,
                                std::string_view companyName)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstringstream ss;
        ss << getWindowPath(FOLDERID_ProgramData) << L"/" << converter.from_bytes(std::string(companyName)) << L"/" << converter.from_bytes(std::string(executableName));
        return ss.str();
#endif
    }

    std::filesystem::path getDataPath(std::string_view executableName,
                                      std::string_view companyName)
    {
        std::filesystem::path programDataPath;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        programDataPath = getDataPathStr(executableName, companyName);
#endif
        if (!std::filesystem::exists(programDataPath))
            std::filesystem::create_directories(programDataPath);
        return programDataPath;
    }

    std::wstring getDocumentFolderPathStr(std::string_view executableName,
                                          std::string_view companyName)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstringstream ss;
        ss << getWindowPath(FOLDERID_Documents) << L"/" << converter.from_bytes(std::string(companyName)) << L"/" << converter.from_bytes(std::string(executableName));
        return ss.str();
#endif
    }

    std::filesystem::path getDocumentFolderPath(std::string_view executableName,
                                                std::string_view companyName)
    {
        std::filesystem::path programDataPath;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        programDataPath = getDocumentFolderPathStr(executableName, companyName);
#endif
        if (!std::filesystem::exists(programDataPath))
            std::filesystem::create_directories(programDataPath);
        return programDataPath;
    }
} // namespace pttkPath