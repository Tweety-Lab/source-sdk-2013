#include "cbase.h"
#include "CSharpScripting.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include "Filesystem.h"

// Static variables
static MonoDomain* s_pDomain = nullptr;
static MonoImage* s_pImage = nullptr;

// Initialize Mono runtime
void CSharpScripting::InitMono()
{
    // Get game.dll path from filesystem
    char fsAssemblyPath[MAX_PATH];
    filesystem->RelativePathToFullPath("bin/x64/Game.dll", "GAME", fsAssemblyPath, sizeof(fsAssemblyPath));

    // Get Mono library path from filesystem
    char fsMonoLibPath[MAX_PATH];
    filesystem->RelativePathToFullPath("../../src/thirdparty/mono/lib", "GAME", fsMonoLibPath, sizeof(fsMonoLibPath));

    // Get Mono etc path from filesystem
    char fsMonoEtcPath[MAX_PATH];
    filesystem->RelativePathToFullPath("../../src/thirdparty/mono/etc", "GAME", fsMonoEtcPath, sizeof(fsMonoEtcPath));

    // Set Mono directories
    mono_set_dirs(fsMonoLibPath, fsMonoEtcPath);

    // Initialize Mono domain
    s_pDomain = mono_jit_init("SourceDomain");
    if (!s_pDomain)
    {
        DevWarning("[C#] Failed to initialize Mono domain\n");
        return;
    }

    // Load assembly
    MonoAssembly* pAssembly = mono_domain_assembly_open(s_pDomain, fsAssemblyPath);
    if (!pAssembly)
    {
        DevWarning("[C#] Failed to load assembly: 'Game.dll'\n");
        return;
    }

    s_pImage = mono_assembly_get_image(pAssembly);
    DevMsg("[C#] Mono domain initialized successfully\n");
}

// Shut down the Mono runtime and clean up resources
void CSharpScripting::CleanupMono()
{
    if (s_pImage)
    {
        mono_image_close(s_pImage);
        s_pImage = nullptr;
    }

    if (s_pDomain)
    {
        mono_jit_cleanup(s_pDomain);
        s_pDomain = nullptr;
    }
}

// Execute a C# method
void CSharpScripting::RunCSharpMethod(const std::string& methodName)
{
    if (!s_pImage)
    {
        DevWarning("[C#] No loaded assembly image\n");
        return;
    }

    MonoMethodDesc* pMethodDesc = mono_method_desc_new(methodName.c_str(), false);
    if (!pMethodDesc)
    {
        DevWarning("[C#] Failed to create method description for: '%s'\n", methodName.c_str());
        return;
    }

    MonoMethod* pMethod = mono_method_desc_search_in_image(pMethodDesc, s_pImage);
    mono_method_desc_free(pMethodDesc);

    if (!pMethod)
    {
        DevWarning("[C#] Method not found: '%s'\n", methodName.c_str());
        return;
    }

    // Invoke the method and handle exceptions
    MonoObject* pException = nullptr;
    mono_runtime_invoke(pMethod, nullptr, nullptr, &pException);

    if (pException)
    {
        MonoString* pExStr = mono_object_to_string(pException, nullptr);
        char* szMsg = mono_string_to_utf8(pExStr);
        DevWarning("[C# Exception] %s\n", szMsg);
        mono_free(szMsg);
    }
}