#pragma once

#include "cbase.h"
#include "wrappers/CSharpEntityWrapper.h"
#include "scripting/CSharpScripting.h"

#ifndef CLIENT_DLL

// Factory class for creating C#-backed entities
class CSharpEntityFactory : public IEntityFactory
{
public:
    IServerNetworkable* Create(const char* pClassName) override;
    void Destroy(IServerNetworkable* pNetworkable) override;
    size_t GetEntitySize() override;
};

#endif // !CLIENT_DLL
