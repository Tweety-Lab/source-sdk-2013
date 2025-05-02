#include "cbase.h"
#include "CSharpEntityFactory.h"

#ifndef CLIENT_DLL


static uint currentHandle = 1;

// Creates a C# entity wrapper based on the class name
IServerNetworkable* CSharpEntityFactory::Create(const char* pClassName)
{
    // Lookup the script class for this entity name
    const char* scriptClassName = static_cast<const char*>(CSharpScripting::GetEntityClassByName(pClassName));

    // Create a new CSharpEntityWrapper
    CSharpEntityWrapper* pEntity = new CSharpEntityWrapper(scriptClassName);
    pEntity->SetNativeHandle(currentHandle++);
    pEntity->PostConstructor(pClassName); // IMPORTANT: We need to call this for the entity to properly register

    // Return its network representation
    return pEntity->NetworkProp();
}

// Destroys the entity and frees its resources
void CSharpEntityFactory::Destroy(IServerNetworkable* pNetworkable)
{
    if (pNetworkable)
        pNetworkable->Release();
}

// Returns the size of the entity
size_t CSharpEntityFactory::GetEntitySize()
{
    return sizeof(CSharpEntityWrapper);
}

#endif // !CLIENT_DLL