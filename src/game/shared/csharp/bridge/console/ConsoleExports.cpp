#include "cbase.h"
#include <vector>

// Manages a registered ConVar and its copied strings, handles cleanup.
struct RegisteredConVar
{

    char* nameCopy;
    char* valueCopy;
    char* descCopy;
    ConVar* var;

    RegisteredConVar(const char* name, const char* value, int flags, const char* description)
    {
        nameCopy = new char[strlen(name) + 1];
        strcpy(nameCopy, name);

        valueCopy = new char[strlen(value) + 1];
        strcpy(valueCopy, value);

        descCopy = new char[strlen(description) + 1];
        strcpy(descCopy, description);

        var = new ConVar(nameCopy, valueCopy, flags, descCopy);

        cvar->RegisterConCommand(var);
    }

    ~RegisteredConVar()
    {
        // Free strings (only if ConVar constructor copies them internally)
        delete[] nameCopy;
        delete[] valueCopy;
        delete[] descCopy;

        // Delete the ConVar to avoid leaks
        delete var;
    }

};

// Store all created ConVars
std::vector<RegisteredConVar*> g_RegisteredConVars;

// Function pointer type for the C# callback
typedef void (*UpdatePropertyCallback)(const char* name, const char* newValue);
UpdatePropertyCallback g_pUpdatePropertyCallback = nullptr;

// ConVar change callback
void ConVarChanged(IConVar* pConVar, const char* pOldValue, float flOldValue)
{
    ConVar* var = static_cast<ConVar*>(pConVar);
    if (g_pUpdatePropertyCallback != nullptr) {
        g_pUpdatePropertyCallback(var->GetName(), var->GetString());
    }
}

// Exported function to be called from C#
// Follows the convention: CS_OriginalMethod
extern "C"
{
    // Logging
    __declspec(dllexport) void CS_Msg(const char* message)
    {
        ConColorMsg(Color(173, 216, 230, 255), "[C#] %s\n", message);
    }

    __declspec(dllexport) void CS_Warning(const char* message)
    {
        ConColorMsg(Color(247, 95, 84, 255), "[C#] %s\n", message);
    }

    // Console Commands
    __declspec(dllexport) void CS_ExecuteCommand(const char* command)
    {
#ifdef CLIENT_DLL
        engine->ClientCmd(command);
#else
        engine->ServerCommand(command);
#endif
    }

    // Console Variables
    __declspec(dllexport) void CS_SetConVar(const char* name, const char* value)
    {
        cvar->FindVar(name)->SetValue(value);
    }

    // Pointer to the C# method to update properties
    void (*CS_UpdatePropertyFromConVar)(const char* name, const char* newValue);

    // Function to handle ConVar change
    __declspec(dllexport) void CS_ConVarChangedCallback(ConVar* pConVar, const char* oldValue, float flOldValue)
    {
        // Get the updated value
        const char* newValue = pConVar->GetString();

        // Call the C# method to update the corresponding property in C#
        if (CS_UpdatePropertyFromConVar != nullptr) {
            CS_UpdatePropertyFromConVar(pConVar->GetName(), newValue);
        }
    }

    // Creates, registers, and stores a ConVar for safe cleanup later.
    __declspec(dllexport) void CS_RegisterConVar(const char* name, const char* value, int flags, const char* description)
    {
        if (!g_pCVar) return;

        RegisteredConVar* newVar = new RegisteredConVar(name, value, flags, description);
        g_RegisteredConVars.push_back(newVar);

    }

    // Not sure if this was necessary to have here specifically but I left it as an example
    __declspec(dllexport) void CS_CleanupConVars()
    {
        for (auto var : g_RegisteredConVars) {
            delete var;
        }
        g_RegisteredConVars.clear();
    }
}