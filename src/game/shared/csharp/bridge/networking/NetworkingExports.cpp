#include "cbase.h"

// Exported function to be called from C#
// Follows the convention: CS_OriginalMethod
extern "C"
{
    __declspec(dllexport) bool CS_IsRunningOnClient()
    {
#ifdef CLIENT_DLL
		return true;
#else
		return false;
#endif

    }
}