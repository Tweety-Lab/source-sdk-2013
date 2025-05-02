#pragma once
#include "scripting/IScriptingBackend.h"
#include <string>
#include <unordered_map>
#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <assert.h>

class DotNetHostBackend : public IScriptingBackend
{
private:
    // HostFxr function pointers
    hostfxr_initialize_for_runtime_config_fn m_initForConfig = nullptr;
    hostfxr_get_runtime_delegate_fn m_getDelegate = nullptr;
    hostfxr_close_fn m_close = nullptr;

    // CoreCLR hosting members
    hostfxr_handle m_hostContext = nullptr;
    load_assembly_and_get_function_pointer_fn m_loadAssemblyFunc = nullptr;


    // Assembly management
    std::string m_assemblyPath;
    std::string m_runtimeConfigPath;
    bool m_initialized = false;

    // Entity management
    std::unordered_map<std::string, const char*> m_entityClasses;

    // Helper methods
    void* LoadLib(const char_t* path);
    void* GetExport(void* hModule, const char* name);
    bool LoadHostFxr();
    bool InitializeHost();
    load_assembly_and_get_function_pointer_fn GetLoadAssemblyFunction(const char_t* configPath);

public:
    static constexpr const char* DefaultAssemblyPath = "SourceEngine.dll";
    static constexpr const char* DefaultRuntimeConfigPath = "SourceEngine.runtimeconfig.json";

    DotNetHostBackend(
        const std::string& assemblyPath = DefaultAssemblyPath,
        const std::string& runtimeConfigPath = DefaultRuntimeConfigPath
    );
    ~DotNetHostBackend() override;

    // IScriptingBackend implementation
    bool Initialize() override;
    void Shutdown() override;

    ScriptDomain* GetDomain() override { return reinterpret_cast<ScriptDomain*>(m_hostContext); }
    ScriptImage* GetImage() override { return reinterpret_cast<ScriptImage*>(this); }

    void RunMethod(const std::string& methodName) override;
    void RunMethod(const std::string& methodName, uint32_t arg) override;
    void RunMethod(const std::string& methodName, const char* arg) override;
    void RegisterEntityType(const char* entityName, const char* entityType) override;
    const char* GetEntityClassByName(const std::string& name) override;
    void CleanupEntities() override { m_entityClasses.clear(); }
#ifndef CLIENT_DLL
    void RegisterEntityFactories() override;
#endif
};