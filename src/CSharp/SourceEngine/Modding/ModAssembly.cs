using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace SourceEngine.Modding
{
    /// <summary>
    /// Represents a C# mod assembly.
    /// </summary>
    public class ModAssembly
    {
        public static List<ModAssembly> LoadedMods = new List<ModAssembly>(); // List of loaded Mod assemblies

        public string Path { get; private set; }
        public Assembly Assembly { get; private set; }
        public Type Type { get; private set; }
        public IMod Mod { get; private set; }

        public ModAssembly(string path)
        {
            Path = path;

            var alc = new ModLoadContext(path);
            Assembly = alc.LoadFromAssemblyPath(System.IO.Path.GetFullPath(path));

            // Add to loaded mods
            LoadedMods.Add(this);
        }

        public void Load()
        {
            try
            {
                Console.Msg($"=== Loading Mod {Path} ===");

                Type[] types;
                try
                {
                    types = Assembly.GetTypes();
                }
                catch (ReflectionTypeLoadException ex)
                {
                    types = ex.Types.Where(t => t != null).ToArray();
                    Console.Warning($"Partial load due to: {ex.Message}");
                }

                // Find the entry point
                Type = types.FirstOrDefault(t => typeof(IMod).IsAssignableFrom(t));
                if (Type == null)
                {
                    // None found
                    return;
                }

                // Run entry point
                Console.Msg($"Found Mod Entry Point: {Type.FullName}");
                Mod = (IMod)Activator.CreateInstance(Type);
                Mod.OnLoad();
            }
            catch (Exception ex)
            {
                Console.Warning($"Load failed: {ex}");
            }
        }
    }

}
