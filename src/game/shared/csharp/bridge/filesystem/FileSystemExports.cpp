#include "cbase.h"
#include "filesystem.h"

// Exported function to be called from C#
// Follows the convention: CS_OriginalMethod
extern "C"
{
    __declspec(dllexport) const char* CS_RelativePathToFullPath(const char* relativePath, const char* pathID)
    {
        static char buffer[MAX_PATH];
        filesystem->RelativePathToFullPath(relativePath, pathID, buffer, sizeof(buffer));
        return buffer;
    }
}