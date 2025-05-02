using System;

namespace SourceEngine.Bridge.Events
{
    /// <summary>
    /// Game Events that are called from the Engine.
    /// </summary>
    public static class GameEvents
    {
        // Map Load
        public static Action OnMapLoad { get; set; }
        private static void TriggerMapLoad()
        {
            OnMapLoad?.Invoke();
        }

        // Player Spawn
        public static Action OnPlayerSpawn { get; set; }
        private static void TriggerPlayerSpawn()
        {
            OnPlayerSpawn?.Invoke();
        }

        // Player Die
        public static Action OnPlayerDeath { get; set; }
        private static void TriggerPlayerDeath()
        {
            OnPlayerDeath?.Invoke();
        }

        // Player Use
        public static Action OnPlayerUse { get; set; }
        private static void TriggerPlayerUse()
        {
            OnPlayerUse?.Invoke();
        }
    }
}
