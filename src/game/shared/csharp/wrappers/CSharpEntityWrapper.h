#pragma once

#include "cbase.h"
#include <unordered_map>

#ifndef CLIENT_DLL

// This class wraps a C# entity inside a Source C++ entity.
// It owns a native handle to the C# entity and calls
// C# virtual methods (e.g., Spawn, Think) from the corresponding C++ overrides.
class CSharpEntityWrapper : public CBaseAnimating
{
public:
    typedef CBaseEntity BaseClass;

    // Constructor
    CSharpEntityWrapper(const char* entityNamespace);

    // Override methods
    void Spawn() override;
    void Think() override;

    // Set the native handle for the C# entity
    void SetNativeHandle(uint handle);

    // Set the think interval
    void SetThinkInterval(float interval);

    // Get the think interval
    float GetThinkInterval();

    // Called when the C# side is being destroyed
    void OnCSharpDestroyed() {
        g_entityHandleMap.erase(m_nativeHandle);
    }

    uint32_t GetNativeHandle() const { return m_nativeHandle; }

    // Static method to retrieve a C# wrapper instance based on the handle
    static CSharpEntityWrapper* GetCSharpEntityWrapper(uint handle);

private:
    // The native handle associated with this entity
    uint m_nativeHandle;

    // Static map for handling C# entities based on native handles
    static std::unordered_map<uint, CSharpEntityWrapper*> g_entityHandleMap;

    // How often this entity should think
    float thinkInterval = 0.0f;
};

#endif // !CLIENT_DLL
