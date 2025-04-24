#include "cbase.h"
#include "CSharpScripting.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

MonoDomain* domain = nullptr;
MonoImage* image = nullptr;

// Init Mono runtime
void CSharpScripting::InitMono()
{
    // Set Mono dirs
    mono_set_dirs("D:\\NewSource\\source-sdk-2013\\game\\mod_hl2mp\\src\\thirdparty\\mono\\lib", "D:\\NewSource\\source-sdk-2013\\game\\mod_hl2mp\\src\\thirdparty\\mono\\etc");

    // Initialize mono with domain, assembly and image
	domain = mono_jit_init("Domain");

    MonoAssembly* assembly = mono_domain_assembly_open(domain, "D:\\NewSource\\source-sdk-2013\\game\\mod_hl2mp\\game\\mod_hl2mp\\bin\\x64\\Game.dll"); // Temporary, will move

    // Check if assembly was valid
    if (assembly == nullptr) 
    {
		DevWarning("[C#] Failed to load assembly: 'Game.dll'\n");
		return;
    }

    image = mono_assembly_get_image(assembly);

    // Print to Console
	DevMsg("Mono domain initialized.\n");
}

// Shuts down the Mono runtime and cleans everything
void CSharpScripting::CleanupMono()
{
    // Cleanup domain only if null
    if (domain != nullptr) {
        mono_jit_cleanup(domain);
        domain = nullptr;
    }

    // Cleanup image only if null
	if (image != nullptr) {
		mono_image_close(image);
		image = nullptr;
	}
}

// Run a C# method
void CSharpScripting::RunCSharpMethod(std::string method)
{
    MonoMethodDesc* methodDesc = mono_method_desc_new(method.c_str(), false);
    MonoMethod* monoMethod = mono_method_desc_search_in_image(methodDesc, image); // Finds the method from the description in the load C# image
    mono_method_desc_free(methodDesc); // Cleans up the method description object (no longer needed once the method is found)

    if (monoMethod)
    {
        // Run method and catch any exceptions
        MonoObject* exception = nullptr;
        MonoObject* result = mono_runtime_invoke(monoMethod, nullptr, nullptr, &exception);

        if (exception)
        {
            MonoString* exStr = mono_object_to_string(exception, nullptr);
            char* msg = mono_string_to_utf8(exStr);
            DevWarning("[C# Exception] %s\n", msg);
            mono_free(msg);
        }
	}
    else
    {
		// Method not found
        DevWarning("[C#] Failed to find method: '%s'\n", method.c_str());
    }
}

