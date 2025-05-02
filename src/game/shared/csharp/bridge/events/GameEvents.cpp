#include "cbase.h"
#include "GameEvents.h"

// Global listener instance
static CSharpEventListener g_MapLoadListener;

// Function to initialize the listener
void InitializeMapLoadListener()
{
    g_MapLoadListener.RegisterListener();
}

// Function to shut down the listener
void ShutdownMapLoadListener()
{
    g_MapLoadListener.UnregisterListener();
}
