#include "cbase.h"
#include "Filesystem.h"
#include "scripting/dotnet/DotNetScriptingBackend.h"
#include "CSharpEntityFactory.h"

#ifdef _WIN32
#include <Windows.h>
#include <locale>
#include <codecvt>
#define STR(s) L##s
#else
#include <dlfcn.h>
#define STR(s) s
#endif

#ifdef _WIN32
std::wstring ConvertToWide(const std::string& str) {
    if (str.empty()) return L"";

    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    if (size <= 0) return L"";

    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size);
    result.pop_back(); // Remove null terminator
    return result;
}
#endif

// Define a platform-independent function type for component entry points
typedef int (CORECLR_DELEGATE_CALLTYPE* component_entry_point_fn)(void* arg, int arg_size_in_bytes);

DotNetHostBackend::DotNetHostBackend(const std::string& assemblyPath, const std::string& runtimeConfigPath)
    : m_assemblyPath(assemblyPath), m_runtimeConfigPath(runtimeConfigPath)
{
}

DotNetHostBackend::~DotNetHostBackend()
{
    Shutdown();
}

void* DotNetHostBackend::LoadLib(const char_t* path)
{
#ifdef _WIN32
    HMODULE h = ::LoadLibraryW(path);
    if (h == nullptr) {
        Warning("[C#] Failed to load library: %ls, error: %lu\n", path, GetLastError());
    }
#else
    void* h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
    if (h == nullptr) {
        Warning("[C#] Failed to load library: %s, error: %s\n", path, dlerror());
    }
#endif
    return h;
}

void* DotNetHostBackend::GetExport(void* hModule, const char* name)
{
    if (hModule == nullptr) {
        Warning("[C#] Cannot get export from null module\n");
        return nullptr;
    }

#ifdef _WIN32
    void* f = ::GetProcAddress(static_cast<HMODULE>(hModule), name);
    if (f == nullptr) {
        Warning("[C#] Failed to get function export: %s, error: %lu\n", name, GetLastError());
    }
#else
    void* f = dlsym(hModule, name);
    if (f == nullptr) {
        Warning("[C#] Failed to get function export: %s, error: %s\n", name, dlerror());
    }
#endif
    return f;
}

bool DotNetHostBackend::LoadHostFxr()
{
    get_hostfxr_parameters params{ sizeof(get_hostfxr_parameters), nullptr, nullptr };
    char_t buffer[MAX_PATH];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);

    int rc = get_hostfxr_path(buffer, &buffer_size, &params);
    if (rc != 0)
    {
        Warning("[C#] Failed to get hostfxr path: 0x%x\n", rc);
        return false;
    }

    void* lib = LoadLib(buffer);
    if (!lib)
    {
        return false; // LoadLib already printed an error
    }

    m_initForConfig = (hostfxr_initialize_for_runtime_config_fn)GetExport(lib, "hostfxr_initialize_for_runtime_config");
    m_getDelegate = (hostfxr_get_runtime_delegate_fn)GetExport(lib, "hostfxr_get_runtime_delegate");
    m_close = (hostfxr_close_fn)GetExport(lib, "hostfxr_close");

    if (!m_initForConfig || !m_getDelegate || !m_close)
    {
        Warning("[C#] Failed to get hostfxr exports\n");
        return false;
    }

    return true;
}

load_assembly_and_get_function_pointer_fn DotNetHostBackend::GetLoadAssemblyFunction(const char_t* configPath)
{
    if (!m_getDelegate || !m_initForConfig) {
        Warning("[C#] Host functions not initialized\n");
        return nullptr;
    }

    hostfxr_handle hostContext = nullptr;
    int rc = m_initForConfig(configPath, nullptr, &hostContext);
    if (rc != 0 || !hostContext)
    {
        Warning("[C#] Failed to initialize host: 0x%x\n", rc);
        return nullptr;
    }

    // Store the host context for later use
    m_hostContext = hostContext;

    // Get the load assembly function
    load_assembly_and_get_function_pointer_fn loadAssemblyFunc = nullptr;
    rc = m_getDelegate(
        hostContext,
        hdt_load_assembly_and_get_function_pointer,
        (void**)&loadAssemblyFunc);

    if (rc != 0 || !loadAssemblyFunc)
    {
        Warning("[C#] Failed to get load assembly function: 0x%x\n", rc);
        m_close(hostContext);
        m_hostContext = nullptr;
        return nullptr;
    }

    return loadAssemblyFunc;
}

bool DotNetHostBackend::InitializeHost()
{
    // Convert paths to platform-specific format
#ifdef _WIN32
    std::wstring config_path = ConvertToWide(m_runtimeConfigPath);
    const char_t* config_path_ptr = config_path.c_str();
#else
    const char_t* config_path_ptr = m_runtimeConfigPath.c_str();
#endif

    // Get the load assembly function
    m_loadAssemblyFunc = GetLoadAssemblyFunction(config_path_ptr);
    if (!m_loadAssemblyFunc)
    {
        Warning("[C#] Failed to get load assembly function\n");
        return false;
    }

    return true;
}

bool DotNetHostBackend::Initialize()
{
    if (m_initialized)
        return true;

    // Get full paths
    char fullAssemblyPath[MAX_PATH];
    char fullConfigPath[MAX_PATH];

    if (!filesystem->RelativePathToFullPath(m_assemblyPath.c_str(), "GAMEBIN", fullAssemblyPath, sizeof(fullAssemblyPath)) ||
        !filesystem->RelativePathToFullPath(m_runtimeConfigPath.c_str(), "GAMEBIN", fullConfigPath, sizeof(fullConfigPath)))
    {
        Warning("[C#] Failed to resolve assembly paths\n");
        return false;
    }

    m_assemblyPath = fullAssemblyPath;
    m_runtimeConfigPath = fullConfigPath;

    if (!LoadHostFxr() || !InitializeHost())
    {
        Warning("[C#] Failed to initialize .NET host\n");
        return false;
    }

    m_initialized = true;
    Msg("[C#] Host initialized successfully\n");
    return true;
}

void DotNetHostBackend::Shutdown()
{
    if (m_hostContext)
    {
        m_close(m_hostContext);
        m_hostContext = nullptr;
    }
    m_initialized = false;
    CleanupEntities();
}

// Run a C# Method
// In .NET we call through the `SourceEngine.Bridge.NETHost.HostBridge.InvokeDotNetMethod` dispatch method.
void DotNetHostBackend::RunMethod(const std::string& methodName)
{
    if (!m_initialized || !m_loadAssemblyFunc) {
        Warning("[C#] Host not initialized\n");
        return;
    }

    // Get the assembly name from m_assemblyPath
    size_t lastSlash = m_assemblyPath.find_last_of("/\\");
    std::string filename = (lastSlash != std::string::npos)
        ? m_assemblyPath.substr(lastSlash + 1)
        : m_assemblyPath;

    size_t dotPos = filename.find_last_of('.');
    std::string assemblyName = (dotPos != std::string::npos)
        ? filename.substr(0, dotPos)
        : filename;

    // Build full type name: "SourceEngine.Bridge.NETHost.HostBridge, AssemblyName"
    // This is a bit hard-coded for my taste, maybe make a dedicated HostBridge assembly?
    std::string fullTypeName = "SourceEngine.Bridge.NETHost.HostBridge, " + assemblyName;

#ifdef _WIN32
    std::wstring assemblyPathW = ConvertToWide(m_assemblyPath);
    std::wstring typeNameW = ConvertToWide(fullTypeName);
    std::wstring methodNameW = ConvertToWide("InvokeDotNetMethod");

    const char_t* assemblyPath = assemblyPathW.c_str();
    const char_t* typeNamePtr = typeNameW.c_str();
    const char_t* methodNamePtr = methodNameW.c_str();
#else
    const char_t* assemblyPath = m_assemblyPath.c_str();
    const char_t* typeNamePtr = fullTypeName.c_str();
    const char_t* methodNamePtr = "InvokeDotNetMethod";
#endif

    // Define the function pointer type
    typedef void(CORECLR_DELEGATE_CALLTYPE* DispatcherFn)(void*);

    // Cache the dispatcher in a static variable
    static DispatcherFn dispatcherFunc = nullptr;

    if (!dispatcherFunc)
    {
        void* funcPtr = nullptr;
        int rc = m_loadAssemblyFunc(
            assemblyPath,
            typeNamePtr,
            methodNamePtr,
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            &funcPtr
        );

        if (rc != 0 || !funcPtr) {
            Warning("[C#] Failed to load dispatcher: 0x%08x\n", rc);
            return;
        }

        dispatcherFunc = reinterpret_cast<DispatcherFn>(funcPtr);
    }

    // Call the dispatcher with method name string
    try {
        dispatcherFunc((void*)methodName.c_str());
    }
    catch (...) {
        Warning("[C#] Exception during method invocation\n");
    }
}

void DotNetHostBackend::RunMethod(const std::string& methodName, uint32_t arg)
{
    if (!m_initialized || !m_loadAssemblyFunc) {
        Warning("[C#] Host not initialized\n");
        return;
    }

    // Get the assembly name from m_assemblyPath
    size_t lastSlash = m_assemblyPath.find_last_of("/\\");
    std::string filename = (lastSlash != std::string::npos)
        ? m_assemblyPath.substr(lastSlash + 1)
        : m_assemblyPath;

    size_t dotPos = filename.find_last_of('.');
    std::string assemblyName = (dotPos != std::string::npos)
        ? filename.substr(0, dotPos)
        : filename;

    // Build full type name
    std::string fullTypeName = "SourceEngine.Bridge.NETHost.HostBridge, " + assemblyName;

#ifdef _WIN32
    std::wstring assemblyPathW = ConvertToWide(m_assemblyPath);
    std::wstring typeNameW = ConvertToWide(fullTypeName);
    std::wstring methodNameW = ConvertToWide("InvokeDotNetMethod");

    const char_t* assemblyPath = assemblyPathW.c_str();
    const char_t* typeNamePtr = typeNameW.c_str();
    const char_t* methodNamePtr = methodNameW.c_str();
#else
    const char_t* assemblyPath = m_assemblyPath.c_str();
    const char_t* typeNamePtr = fullTypeName.c_str();
    const char_t* methodNamePtr = "InvokeDotNetMethod";
#endif

    // Define the function pointer type with uint parameter
    typedef void(CORECLR_DELEGATE_CALLTYPE* DispatcherFn)(void*, uint32_t);

    // Cache the dispatcher in a static variable
    static DispatcherFn dispatcherFunc = nullptr;

    if (!dispatcherFunc)
    {
        void* funcPtr = nullptr;
        int rc = m_loadAssemblyFunc(
            assemblyPath,
            typeNamePtr,
            methodNamePtr,
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            &funcPtr
        );

        if (rc != 0 || !funcPtr) {
            Warning("[C#] Failed to load dispatcher: 0x%08x\n", rc);
            return;
        }

        dispatcherFunc = reinterpret_cast<DispatcherFn>(funcPtr);
    }

    // Call the dispatcher with method name string and uint argument
    try {
        dispatcherFunc((void*)methodName.c_str(), arg);
    }
    catch (...) {
        Warning("[C#] Exception during method invocation\n");
    }
}

void DotNetHostBackend::RunMethod(const std::string& methodName, const char* arg)
{
    if (!m_initialized || !m_loadAssemblyFunc) {
        Warning("[C#] Host not initialized\n");
        return;
    }

    // Get the assembly name from m_assemblyPath
    size_t lastSlash = m_assemblyPath.find_last_of("/\\");
    std::string filename = (lastSlash != std::string::npos)
        ? m_assemblyPath.substr(lastSlash + 1)
        : m_assemblyPath;

    size_t dotPos = filename.find_last_of('.');
    std::string assemblyName = (dotPos != std::string::npos)
        ? filename.substr(0, dotPos)
        : filename;

    // Build full type name
    std::string fullTypeName = "SourceEngine.Bridge.NETHost.HostBridge, " + assemblyName;

#ifdef _WIN32
    std::wstring assemblyPathW = ConvertToWide(m_assemblyPath);
    std::wstring typeNameW = ConvertToWide(fullTypeName);
    std::wstring methodNameW = ConvertToWide("InvokeDotNetMethod_Str");

    const char_t* assemblyPath = assemblyPathW.c_str();
    const char_t* typeNamePtr = typeNameW.c_str();
    const char_t* methodNamePtr = methodNameW.c_str();
#else
    const char_t* assemblyPath = m_assemblyPath.c_str();
    const char_t* typeNamePtr = fullTypeName.c_str();
    const char_t* methodNamePtr = "InvokeDotNetMethod";
#endif

    // Define the function pointer type with uint parameter
    typedef void(CORECLR_DELEGATE_CALLTYPE* DispatcherFn)(void*, void*);

    // Cache the dispatcher in a static variable
    static DispatcherFn dispatcherFunc = nullptr;

    if (!dispatcherFunc)
    {
        void* funcPtr = nullptr;
        int rc = m_loadAssemblyFunc(
            assemblyPath,
            typeNamePtr,
            methodNamePtr,
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            &funcPtr
        );

        if (rc != 0 || !funcPtr) {
            Warning("[C#] Failed to load dispatcher: 0x%08x\n", rc);
            return;
        }

        dispatcherFunc = reinterpret_cast<DispatcherFn>(funcPtr);
    }

    // Call the dispatcher with method name string and const char* argument
    try {
        dispatcherFunc((void*)methodName.c_str(), (void*)arg);
    }
    catch (...) {
        Warning("[C#] Exception during method invocation\n");
    }
}

void DotNetHostBackend::RegisterEntityType(const char* entityName, const char* entityNamespace)
{
    if (entityName && entityNamespace)
    {
        m_entityClasses[entityName] = entityNamespace;
        Msg("[C#] Registered entity type: %s\n", entityName);
    }
}

const char* DotNetHostBackend::GetEntityClassByName(const std::string& name)
{
    auto it = m_entityClasses.find(name);
    if (it == m_entityClasses.end())
    {
        return nullptr;
    }

    // return the entity namespace
    return it->second;
}


#ifndef CLIENT_DLL
void DotNetHostBackend::RegisterEntityFactories()
{
    for (const auto& pair : m_entityClasses)
    {
        const std::string& entityName = pair.first;
        EntityFactoryDictionary()->InstallFactory(new CSharpEntityFactory(), entityName.c_str());

        Msg("[C#] Factory registered for: %s\n", entityName.c_str());
    }
}
#endif