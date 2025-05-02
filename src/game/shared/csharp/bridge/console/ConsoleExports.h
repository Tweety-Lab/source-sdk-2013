#pragma once

#ifdef _WIN32
#define CS_EXPORT __declspec(dllexport)
#else
#define CS_EXPORT
#endif

extern "C"
{
    CS_EXPORT void CS_Msg(const char* message);
	CS_EXPORT void CS_Warning(const char* message);
	CS_EXPORT void CS_ExecuteCommand(const char* command);
	CS_EXPORT void CS_SetConVar(const char* name, const char* value);
	CS_EXPORT void CS_RegisterConVar(const char* name, const char* value, int flags, const char* description);

}
