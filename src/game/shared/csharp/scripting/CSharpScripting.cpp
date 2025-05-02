#include "cbase.h"
#include "CSharpScripting.h"

// Static members
std::unique_ptr<IScriptingBackend> CSharpScripting::s_pBackend;
bool CSharpScripting::IsInitialized = false;