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
    // Load domain, assembly and image.
    domain = mono_jit_init("CSDomain");

    MonoAssembly* assembly = mono_domain_assembly_open(domain, "Game.dll"); // Temporary, will move
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
        mono_runtime_invoke(monoMethod, nullptr, nullptr, nullptr); // Calls the method
    }
}

