#include "cbase.h"

#include "scripting/CSharpScripting.h"
#include "wrappers/CSharpEntityWrapper.h"

// Exported function to be called from C#
// Follows the convention: CS_OriginalMethod
extern "C"
{
    __declspec(dllexport) void CS_RegisterCSharpEntity(const char* entityName, void* type)
    {
        if (!entityName || !type)
        {
            Warning("Failed to register C# entity: Invalid name or type");
            return;
        }

        CSharpScripting::RegisterEntityType(entityName, type);
    }

#ifndef CLIENT_DLL
    _declspec(dllexport) void CS_SetEntityModel(uint entityHandle, const char* modelName)
    {
        // Get entity from handle
		CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

        // Precache and set model
        CBaseEntity::PrecacheModel(modelName);
        pEntity->SetModel(modelName);
    }

    _declspec(dllexport) string_t CS_GetEntityModel(uint entityHandle) 
    {
		// Get entity from handle
		CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);
        return pEntity->GetModelName();
    }

    _declspec(dllexport) void CS_SetEntityAbsOrigin(uint entityHandle, Vector* position)
    {
        // Get entity from handle
        CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

        pEntity->SetAbsOrigin(*position);
    }

    _declspec(dllexport) void CS_GetEntityAbsOrigin(uint entityHandle, Vector* outPosition)
    {
        // Get entity from handle
        CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

        if (pEntity && outPosition)
        {
            *outPosition = pEntity->GetAbsOrigin();
        }
    }

    _declspec(dllexport) void CS_SetEntityLocalOrigin(uint entityHandle, Vector* position)
    {
        // Get entity from handle
        CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

        pEntity->SetLocalOrigin(*position);
    }

    _declspec(dllexport) void CS_GetEntityLocalOrigin(uint entityHandle, Vector* outPosition)
    {
        // Get entity from handle
        CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

        if (pEntity && outPosition)
        {
            *outPosition = pEntity->GetLocalOrigin();
        }
    }

    _declspec(dllexport) void CS_SetEntityThinkInterval(uint entityHandle, float interval) 
    {
		// Get entity from handle
		CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

        // Set think interval
        pEntity->SetThinkInterval(interval);
    }

    _declspec(dllexport) void CS_GetEntityThinkInterval(uint entityHandle, float outInterval)
    {
        // Get entity from handle
        CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

        if (pEntity && outInterval)
        {
            outInterval = pEntity->GetThinkInterval();
        }
    }

    _declspec(dllexport) void CS_SetEntitySolidType(uint entityHandle, SolidType_t solidType)
    {
        // Get entity from handle
        CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

        // Set solid type
        pEntity->SetSolid(solidType);
    }

    _declspec(dllexport) void CS_SetEntityMoveType(uint entityHandle, MoveType_t moveType) 
    {
		// Get entity from handle
		CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

		// Set move type
		pEntity->SetMoveType(moveType);
    }

    _declspec(dllexport) void CS_InitEntityPhysics(uint entityHandle)
    {
        // Get entity from handle
        CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

		// Initialize physics
		pEntity->VPhysicsInitNormal(SOLID_VPHYSICS, 0, false);
    }

    _declspec(dllexport) void CS_SetEntityAbsVelocity(uint entityHandle, Vector* velocity)
    {
        // Get entity from handle
        CSharpEntityWrapper* pEntity = CSharpEntityWrapper::GetCSharpEntityWrapper(entityHandle);

        IPhysicsObject* pPhysics = pEntity->VPhysicsGetObject();
        if (pPhysics)
        {
            pPhysics->SetVelocityInstantaneous(velocity, nullptr);
        }
        else 
        {
			pEntity->SetAbsVelocity(*velocity);
        }
    }
#endif // !CLIENT_DLL
}