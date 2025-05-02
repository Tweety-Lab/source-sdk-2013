#include "cbase.h"
#include "CSharpEntityWrapper.h"
#include "csharp/scripting/CSharpScripting.h"
#include <unordered_map>

#ifndef CLIENT_DLL

// Define the static map for handling C# entities based on native handles
std::unordered_map<uint32_t, CSharpEntityWrapper*> CSharpEntityWrapper::g_entityHandleMap;

// C# Wrapper for BaseEntity
CSharpEntityWrapper::CSharpEntityWrapper(const char* entityNamespace)
{
    // Create C# Entity
	CSharpScripting::RunCSharpMethod("SourceEngine.Entities.EntitySystem:Create", entityNamespace);
}

void CSharpEntityWrapper::SetNativeHandle(uint handle)
{
    m_nativeHandle = handle;

    // Store the wrapper instance in the global handle map
    g_entityHandleMap[handle] = this;
}

void CSharpEntityWrapper::Spawn()
{
    // Call C# Spawn
    CSharpScripting::RunCSharpMethod("SourceEngine.Entities.EntitySystem:Spawn", m_nativeHandle);

    // Schedule thinking
    SetNextThink(gpGlobals->curtime + thinkInterval);
}

void CSharpEntityWrapper::Think()
{
    // Call Base Think first
    BaseClass::Think();

    // Call C# Think
	CSharpScripting::RunCSharpMethod("SourceEngine.Entities.EntitySystem:Think", m_nativeHandle);

    // Schedule next think
    SetNextThink(gpGlobals->curtime + thinkInterval);
}

void CSharpEntityWrapper::SetThinkInterval(float interval)
{
	thinkInterval = interval;
}

float CSharpEntityWrapper::GetThinkInterval()
{
	return thinkInterval;
}

// Retrieve a C# wrapper instance based on the handle
CSharpEntityWrapper* CSharpEntityWrapper::GetCSharpEntityWrapper(uint handle)
{
    auto it = g_entityHandleMap.find(handle);
    return (it != g_entityHandleMap.end()) ? it->second : nullptr;
}

#endif // !CLIENT_DLL
