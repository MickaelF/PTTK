#include "pttk/pttkpath.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <ShlObj.h>
#include <sstream>
#endif

namespace
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    std::wstring getWindowPath(const GUID folderId)
    {
        wchar_t *path{nullptr};
        SHGetKnownFolderPath(FOLDERID_ProgramData, KF_FLAG_DEFAULT,
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
    std::wstring getDataPathStr(std::wstring_view executableName,
                                std::wstring_view companyName)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        std::wstringstream ss;
        ss << getWindowPath(FOLDERID_ProgramData) << L"/" << companyName << L"/" << executableName;
        return ss.str();
#endif
    }

    std::filesystem::path getDataPath(std::wstring_view executableName,
                                      std::wstring_view companyName)
    {
        std::filesystem::path programDataPath;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        programDataPath = getDataPathStr(executableName, companyName);
#endif
        if (!std::filesystem::exists(programDataPath))
            std::filesystem::create_directories(programDataPath);
        return programDataPath;
    }

    std::wstring getDocumentFolderPathStr(std::wstring_view executableName,
                                          std::wstring_view companyName)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        std::wstringstream ss;
        ss << getWindowPath(FOLDERID_Documents) << L"/" << companyName << L"/" << executableName;
        return ss.str();
#endif
    }

    std::filesystem::path getDocumentFolderPath(std::wstring_view executableName,
                                                std::wstring_view companyName)
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