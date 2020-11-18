#include "pttk/pttkpath.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #include <ShlObj.h>
#endif
namespace pttkPath
{
std::filesystem::path getDataPath(std::string_view executableName, std::string_view companyName)
{
    std::filesystem::path programDataPath;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    PWSTR path {NULL};
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramData, KF_FLAG_DEFAULT, NULL, &path);
    if (SUCCEEDED(hr)) programDataPath = path;
#endif
    programDataPath.append(std::string(companyName) + "/" + std::string(executableName));
    if (!std::filesystem::exists(programDataPath))
        std::filesystem::create_directories(programDataPath);
    return programDataPath;
}
} // namespace pttkPath