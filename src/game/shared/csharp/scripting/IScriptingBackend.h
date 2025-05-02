#pragma once
#include <string>
#include <unordered_map>

// Forward declare opaque handle types
struct ScriptDomain;
struct ScriptImage;

class IScriptingBackend
{
public:
    virtual ~IScriptingBackend() = default;

    // Lifecycle management
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;

    // Domain/assembly access
    virtual ScriptDomain* GetDomain() = 0;
    virtual ScriptImage* GetImage() = 0;

    // Method execution
    virtual void RunMethod(const std::string& methodName) = 0;
    virtual void RunMethod(const std::string& methodName, uint32_t uintArg) = 0;
    virtual void RunMethod(const std::string& methodName, const char* stringArg) = 0;

    // Entity management
    virtual void RegisterEntityType(const char* entityName, const char* entityType) = 0;
    virtual const char* GetEntityClassByName(const std::string& name) = 0;
    virtual void CleanupEntities() = 0;

    // Factory registration
    virtual void RegisterEntityFactories() = 0;
};