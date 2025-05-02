#pragma once

#ifdef _WIN32
#define CS_EXPORT __declspec(dllexport)
#else
#define CS_EXPORT
#endif

extern "C"
{
    CS_EXPORT const char* CS_RelativePathToFullPath(const char* relativePath, const char* pathID);
}
