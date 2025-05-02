using SourceEngine.Bridge.Events;
using SourceEngine.Entities;
using SourceEngine.Modding;
using System.IO;
using System;

namespace SourceEngine
{
    // Handle loading C# stuff
    public static class SourceEngine
    {
        // Load Game DLLs
        public static void Load()
        {
            Console.Msg("Loading Game DLLs...");

            // Search for Game.dll
            if (File.Exists(FileSystem.RelativePathToFullPath("Game.dll", PathID.GameBin)))
            {
                ModAssembly game = new ModAssembly(FileSystem.RelativePathToFullPath("Game.dll", PathID.GameBin));

                // Load Game.dll
                game.Load();
            }

            // Bind to a bunch of game events for testing
            GameEvents.OnMapLoad += () =>
            {
                Console.Msg($"Player loaded into a new map");
            };

            GameEvents.OnPlayerSpawn += () =>
            {
                Console.Msg($"Player spawned");
            };

            GameEvents.OnPlayerDeath += () =>
            {
                Console.Msg($"Player died");
            };

            GameEvents.OnPlayerUse += () =>
            {
                Console.Msg($"Player used an entity");
            };

        }
    }
}
