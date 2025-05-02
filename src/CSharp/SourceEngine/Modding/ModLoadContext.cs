using SourceEngine.Modding;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Loader;

public class ModLoadContext : AssemblyLoadContext
{
    private readonly string _modPath;

    public ModLoadContext(string modPath) : base(isCollectible: false)
    {
        _modPath = modPath;
    }

    protected override Assembly? Load(AssemblyName assemblyName)
    {
        // Always use the host's SourceEngine assembly
        if (assemblyName.Name == "SourceEngine")
        {
            return typeof(IMod).Assembly; // Force use the host's version
        }

        // For other assemblies, try default context first
        var defaultAssembly = AssemblyLoadContext.Default.Assemblies
            .FirstOrDefault(a => a.GetName().Name == assemblyName.Name);
        if (defaultAssembly != null) return defaultAssembly;

        // Fallback to mod directory
        var modDir = Path.GetDirectoryName(_modPath)!;
        var candidatePath = Path.Combine(modDir, $"{assemblyName.Name}.dll");
        return File.Exists(candidatePath)
            ? LoadFromAssemblyPath(candidatePath)
            : null;
    }

}
