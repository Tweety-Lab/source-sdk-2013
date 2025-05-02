#include "cbase.h"

#include "inputsystem/iinputsystem.h"


// Exported function to be called from C#
// Follows the convention: CS_OriginalMethod
extern "C"
{
#ifdef CLIENT_DLL
	__declspec(dllexport) bool CS_GetButtonDown(ButtonCode_t button)
	{
		// Check if the button is down
		if (inputsystem->IsButtonDown(button))
			return true;

		// Button is not down
		return false;
	}
#endif // CLIENT_DLL
}