#pragma once

#include "cbase.h"
#include "igameevents.h"
#include "scripting/CSharpScripting.h"

// Create the C++ event listener class
class CSharpEventListener : public IGameEventListener2
{
public:
    CSharpEventListener() {}
    virtual ~CSharpEventListener() {}

    void FireGameEvent(IGameEvent* event) override
    {
        // New Map
        if (FStrEq(event->GetName(), "game_newmap"))
        {
            CSharpScripting::RunCSharpMethod("SourceEngine.Bridge.Events.GameEvents:TriggerMapLoad");
        }

        // Player Spawn
		else if (FStrEq(event->GetName(), "player_spawn"))
		{
			CSharpScripting::RunCSharpMethod("SourceEngine.Bridge.Events.GameEvents:TriggerPlayerSpawn");
		}

        // Player Die
		else if (FStrEq(event->GetName(), "player_death"))
		{
			CSharpScripting::RunCSharpMethod("SourceEngine.Bridge.Events.GameEvents:TriggerPlayerDeath");
		}

        // Player Use
		else if (FStrEq(event->GetName(), "player_use"))
		{
			CSharpScripting::RunCSharpMethod("SourceEngine.Bridge.Events.GameEvents:TriggerPlayerUse");
		}
    }

    // Register this listener
    void RegisterListener()
    {
        gameeventmanager->AddListener(this, "game_newmap", false);
		gameeventmanager->AddListener(this, "player_spawn", false);
		gameeventmanager->AddListener(this, "player_death", false);
		gameeventmanager->AddListener(this, "player_use", false);
    }

    // Unregister the listener
    void UnregisterListener()
    {
        gameeventmanager->RemoveListener(this);
    }
};
