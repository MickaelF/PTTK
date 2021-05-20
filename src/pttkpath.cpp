#include "pttk/pttkpath.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <ShlObj.h>
#include <sstream>
#include <locale>
#include <codecvt>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string>
#endif

namespace
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    std::wstring getIdPath(const GUID folderId)
    {
        wchar_t *path{nullptr};
        SHGetKnownFolderPath(folderId, KF_FLAG_DEFAULT,
                             NULL, &path);
        std::wstringstream ss;
        ss << path;
        CoTaskMemFree(static_cast<void *>(path));
        return ss.str();
    }

    std::filesystem::path getWindowsPath(GUID id, std::string_view executableName,
                                         std::string_view companyName)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstringstream ss;
        ss << getIdPath(id) << L"/" << converter.from_bytes(std::string(companyName)) << L"/" << converter.from_bytes(std::string(executableName));
        std::filesystem::path programDataPath{ss.str()};
        if (!std::filesystem::exists(programDataPath))
            std::filesystem::create_directories(programDataPath);
        return programDataPath;
    }
#else
    std::filesystem::path getLinuxPath(std::string_view folderName, std::string_view executableName, std::string_view companyName)
    {
        const char *home = getenv("HOME");
        if (!home)
        {
            passwd *p{getpwuid(getuid())};
            home = p->pw_dir;
        }
        std::string strPath{home + std::string{folderName} + std::string{companyName} + '/' + std::string{executableName}};
        return std::filesystem::path{strPath};
    }
#endif
}

namespace pttkPath
{
    std::filesystem::path getDataPath(std::string_view executableName,
                                      std::string_view companyName)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        return getWindowsPath(FOLDERID_ProgramData, executableName, companyName);
#else // Linux
        return getLinuxPath("/.local/", executableName, companyName);
#endif
    }

    std::filesystem::path getDocumentFolderPath(std::string_view executableName,
                                                std::string_view companyName)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        return getWindowsPath(FOLDERID_Documents, executableName, companyName);
#else // Linux
        return getLinuxPath("/Documents/", executableName, companyName);
#endif
    }
} // namespace pttkPath