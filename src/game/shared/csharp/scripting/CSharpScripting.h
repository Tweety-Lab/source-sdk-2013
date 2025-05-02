#pragma once
#pragma once
#include "IScriptingBackend.h"
#include <memory>

class CSharpScripting
{
private:
    static std::unique_ptr<IScriptingBackend> s_pBackend;

public:
    static bool IsInitialized;

    // Initialization with specific backend
    template<typename T>
    static void Initialize()
    {
        if (!s_pBackend)
        {
            s_pBackend = std::make_unique<T>();
            s_pBackend->Initialize();

            // Update isinitialized
			IsInitialized = true;
        }
    }


    static void Shutdown()
    {
        if (s_pBackend)
        {
            s_pBackend->Shutdown();
            s_pBackend.reset();
        }
    }

    // Forward all other calls to the backend
    static void RunCSharpMethod(const std::string& method)
    {
        if (s_pBackend) s_pBackend->RunMethod(method);
    }

    static void RunCSharpMethod(const std::string& method, uint32_t handle) {
        if (s_pBackend) s_pBackend->RunMethod(method, handle);
    }

    static void RunCSharpMethod(const std::string& method, const char* string) {
        if (s_pBackend) s_pBackend->RunMethod(method, string);
    }


    static void RegisterEntityType(const char* name, void* typePtr)
    {
        if (s_pBackend)
        {
            const char* typeName = static_cast<const char*>(typePtr);
            s_pBackend->RegisterEntityType(name, typeName);
        }
    }

    static const char* GetEntityClassByName(const std::string& name)
    {
        return s_pBackend ? s_pBackend->GetEntityClassByName(name) : nullptr;
    }

    static void RegisterCSharpEntityFactories()
    {
        if (s_pBackend) s_pBackend->RegisterEntityFactories();
    }

    static void CleanupCSharpEntities()
    {
        if (s_pBackend) s_pBackend->CleanupEntities();
    }
};